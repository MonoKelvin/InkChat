#include "MessageManager.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <ChatList.h>
#include <MessageDatabase.h>
#include <User.h>

#include <QDataStream>

#include <QDateTime>
#include <QUdpSocket>

QHash<int, QByteArray> MessageManager::mRegistryChatClasses;

MessageManager::MessageManager(QObject* parent)
    : QObject(parent)
    , mPort(LAN_UDP_PORT)
{
    mUdpSocket = new QUdpSocket(this);

    if (mUdpSocket->bind(mPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        connect(mUdpSocket, &QUdpSocket::readyRead, this, &MessageManager::processPendingDatagrams);
    } else {
        mUdpSocket->deleteLater();
    }
}

MessageManager::~MessageManager()
{
    qDebug() << "MessageManager Destroyed";
}

template <typename T>
void MessageManager::RegisterChatItemClass()
{
    const auto metaClass = T::staticMetaObject;

    Q_ASSERT(metaClass.inherits(&AbstractChatListItem::staticMetaObject));

    if (mRegistryChatClasses.contains(T::ChatType)) {
        qWarning("AbstractChatListItem类型已存在，原类型将会被覆盖");
    }

    qRegisterMetaType<T>(metaClass.className());
    mRegistryChatClasses.insert(T::ChatType, metaClass.className());
}

ChatItem* MessageManager::BuildChatItem(int chatType, const SUserChatData& userData, const QVariant& data)
{
    if (userData.isEmpty()) {
        return nullptr;
    }

    const auto& className = mRegistryChatClasses.value(chatType);
    const auto& id = QMetaType::type(className);

    // 通过反射机制创建实例
    ChatItem* chat = nullptr;
    if (id != QMetaType::UnknownType) {
        chat = static_cast<ChatItem*>(QMetaType::create(id));
        if (!chat) {
            return nullptr;
        }

        chat->fromSenderData(userData);
        chat->setData(data);
    }

    return chat;
}

ChatItem* MessageManager::BuildChatItem(int chatType, const QVariant& data)
{
    return BuildChatItem(chatType, User::Instance()->getChatData(), data);
}

void MessageManager::sendMessage(ChatList* view, int type, const QVariant& data)
{
    if (!data.isValid() || data.isNull()) {
        emit failed(tr("要发送的消息无效，请尝试重新发送"));
        return;
    }

    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    const auto& chatObj = view->mChatObject.data();
    const auto& time = QDateTime::currentDateTime();
    const auto& item = MessageManager::BuildChatItem(type, data);

    if (nullptr == item) {
        emit failed(tr("消息构建失败，请尝试重新发送"));
        return;
    }

    // 填充数据
    out << User::Instance()->getUuid();
    out << User::Instance()->getHostAddress();
    out << User::Instance()->getNickName();
    out << chatObj->getRoleType();
    out << type;
    out << time;
    out << data;

    // 发送数据
    qint64 result = -1;
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        // md5用于验证同一个多人聊天对象
        //out << chatObj->getMD5();
        result = mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, mPort);
    } else {
        result = mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), mPort);
    }

    if (result < 0) {
        emit failed(tr("消息发送失败，请再试一遍"));
        qDebug() << mUdpSocket->errorString();
        return;
    }

    // 显示到视图中并保存到本地数据库
    item->setTime(time);
    item->setSendState(ChatItem::Succeed);
    view->appendItem(item);
    MessageDatabase::Instance()->saveAChatRecord(item, chatObj);
}

void MessageManager::loadChatRecords(ChatList* view)
{
    MessageDatabase::Instance()->loadChatItems(view);
}

void MessageManager::saveAChatRecord(ChatList* view, ChatItem* item) const
{
    MessageDatabase::Instance()->saveAChatRecord(item, view->mChatObject.data());
}

void MessageManager::processPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        // 读数据
        const int dataSize = int(mUdpSocket->pendingDatagramSize());
        if (dataSize <= 0) {
            continue;
        }
        datagram.resize(dataSize);
        mUdpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        // 首先判断是否是自己发送的数据，如果是则不接收
        SChatItemPackage package;
        in >> package.UserChatData.Uuid;
        if (package.UserChatData.Uuid == User::Instance()->getUuid()) {
            continue;
        }

        // 处理数据
        in >> package.HostAddress >> package.UserChatData.Name
            >> package.RoleType >> package.ChatType >> package.Time >> package.Data;

        emit received(package);
    }
}

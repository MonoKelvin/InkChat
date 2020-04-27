#include "MessageManager.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <ChatList.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
//#include <NotificationItem.h>
#include <TcpServer.h>
#include <User.h>

#include <QUdpSocket>

QHash<int, QByteArray> MessageManager::mRegistryChatClasses;

inline const QString _getUserBehavior(int type, const QString& name)
{
    if (type == AbstractChatListItem::UserJoin) {
        return QObject::tr("用户 %1 加入").arg(name);
    }
    if (type == AbstractChatListItem::UserLeft) {
        return QObject::tr("用户 %1 离开").arg(name);
    }

    return QString();
}

MessageManager::MessageManager(QObject* parent)
    : QObject(parent)
{
    mUdpSocket = new QUdpSocket(this);

    if (mUdpSocket->bind(LAN_UDP_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        connect(mUdpSocket, &QUdpSocket::readyRead, this, &MessageManager::processPendingDatagrams);
    } else {
        mUdpSocket->deleteLater();
    }
}

MessageManager::~MessageManager()
{
    qDebug() << "MessageManager Destroyed";
}

ChatItem* MessageManager::BuildChatItem(int chatType, const SChatItemData& chatData)
{
    if (chatData.isEmpty()) {
        return nullptr;
    }

    const auto& className = mRegistryChatClasses.value(chatType);
    const auto& id = QMetaType::type(className);

    // 通过反射机制创建实例
    ChatItem* chat = nullptr;
    if (id != QMetaType::UnknownType) {
        chat = static_cast<ChatItem*>(QMetaType::create(id));
        if (chat) {
            chat->setChatItemData(chatData);
        }
    }

    return chat;
}

ChatItem* MessageManager::BuildChatItem(int chatType, const QVariant& data)
{
    return BuildChatItem(chatType, User::Instance()->getChatData(data));
}

AbstractChatListItem* MessageManager::BuildChatListItem(int chatType, const QVariant& data)
{
    const auto& className = mRegistryChatClasses.value(chatType);
    const auto& id = QMetaType::type(className);

    AbstractChatListItem* item = nullptr;
    if (id != QMetaType::UnknownType) {
        item = static_cast<ChatItem*>(QMetaType::create(id));
        if (item) {
            item->setData(data);
        }
    }

    return item;
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
    ChatItem* item = BuildChatItem(type, data);
    if (!item) {
        emit failed(tr("消息构建失败，请尝试重新发送"));
        return;
    }

    if (type & AbstractChatListItem::TCP_Protocol) {
        // 新建TCP服务器
        TcpServer* server = new TcpServer(this);
        if (!server->setFileToSend(data.toString(), item)) {
            emit failed(tr("发送失败，请重新尝试"));
            delete item;
            item = nullptr;
            server = nullptr;
            return;
        }
    }

    // 填充数据
    out << User::Instance()->getUuid();
    out << User::Instance()->getNickName();
    out << chatObj->getHostAddress();
    out << type;

    /*if (type & AbstractChatListItem::UserBehavior) {
        NotificationItem* item = new NotificationItem(_getUserBehavior(type, User::Instance()->getNickName()));
        view->appendItem(item);
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, LAN_UDP_PORT);
        return;
    }*/

    out << chatObj->getRoleType();
    out << data;

    // 尽早显示到视图中
    item->setTime(QDateTime::currentDateTime());
    //item->setSendState(ChatItem::Succeed);
    view->appendItem(item);

    // 发送数据
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, LAN_UDP_PORT);
    } else {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), LAN_UDP_PORT);
    }

    // 保存到本地数据库
    MessageDatabase::Instance()->saveAChatRecord(item, view->getChatObject()->getUuid());
}

void MessageManager::loadChatRecords(ChatList* view)
{
    MessageDatabase::Instance()->loadChatItems(view);
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

        // 有可能是自己发送的数据
        SChatItemPackage package;
        in >> package.UserChatData.Uuid >> package.UserChatData.Name
            >> package.HostAddress >> package.ChatType;

        /*if (package.ChatType == AbstractChatListItem::UserJoin) {
            emit userJoin(new NotificationItem(tr("用户 %1 加入").arg(package.UserChatData.Name)));
            return;
        } else if (package.ChatType == AbstractChatListItem::UserLeft) {
            emit userJoin(new NotificationItem(tr("用户 %1 离开").arg(package.UserChatData.Name)));
            return;
        }*/

        in >> package.RoleType >> package.UserChatData.Message;

        emit received(package);
    }
}

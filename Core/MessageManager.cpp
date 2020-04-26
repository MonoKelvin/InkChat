#include "MessageManager.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <ChatList.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
#include <TcpClient.h>
#include <TcpServer.h>
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
        if (!chat) {
            return nullptr;
        }

        chat->setChatItemData(chatData);
    }

    return chat;
}

ChatItem* MessageManager::BuildChatItem(int chatType, const QVariant& msg)
{
    return BuildChatItem(chatType, User::Instance()->getChatData(msg));
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
    const auto& item = MessageManager::BuildChatItem(type, data);

    if (nullptr == item) {
        emit failed(tr("消息构建失败，请尝试重新发送"));
        return;
    }

    // 填充数据
    out << User::Instance()->getUuid();
    out << view->getChatObject()->getHostAddress();
    out << User::Instance()->getNickName();
    out << chatObj->getRoleType();
    out << type;

    // 如果要进行TCP类型通信，就新建TCP服务器
    if (type & ChatItem::TCPP_Protocol) {
        TcpServer* server = new TcpServer(this);
        connect(server, &TcpServer::failed, [this](const QString& msg) { emit failed(msg); });
        server->setFileToSend(data.toString(), item);
        out << getFileNameFromPath(data.toString());
    } else {
        out << data;
    }

    // 发送数据
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, mPort);
    } else {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), mPort);
    }

    // 显示到视图中并保存到本地数据库
    item->setTime(QDateTime::currentDateTime());
    item->setSendState(ChatItem::Succeed);
    view->appendItem(item);
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
        in >> package.UserChatData.Uuid >> package.HostAddress >> package.UserChatData.Name
            >> package.RoleType >> package.ChatType >> package.UserChatData.Message;

        emit received(package);
    }
}

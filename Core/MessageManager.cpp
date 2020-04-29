#include "MessageManager.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <ChatList.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
#include <NotificationItem.h>
#include <TcpServer.h>
#include <User.h>

#include <QUdpSocket>

QHash<int, QByteArray> MessageManager::mRegistryChatClasses;

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

ChatItem* MessageManager::BuildChatItem(int chatType, const SUserBaseData& chatData)
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
    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    const auto& chatObj = view->mChatObject.data();
    ChatItem* item = nullptr;

    out << type;
    out << User::Instance()->getUuid();
    out << User::Instance()->getNickName();
    out << chatObj->getHostAddress();

    item = BuildChatItem(type, data);
    item->setTime(QDateTime::currentDateTime());
    //item->setSendState(ChatItem::Succeed);
    view->appendItem(item);

    switch (type) {
    case AbstractChatListItem::File: {
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
    case AbstractChatListItem::Text:
        // 填充数据
        out << chatObj->getRoleType();
        out << data;

        // 保存到本地数据库
        MessageDatabase::Instance()->saveAChatRecord(item, view->getChatObject()->getUuid());

        break;
    default:
        break;
    }

    // 发送数据
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, LAN_UDP_PORT);
    } else {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), LAN_UDP_PORT);
    }

    // TODO: 如果发送失败
    //item->setSendState(ChatItem::Failed);
}

void MessageManager::sendUserBehavior(const QString& addr, int type)
{
    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    out << type;
    out << User::Instance()->getUuid();
    out << User::Instance()->getNickName();
    out << addr;

    mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, LAN_UDP_PORT);
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
        in >> package.ChatType >> package.UserChatData.Uuid
            >> package.UserChatData.Name >> package.HostAddress;

        switch (package.ChatType) {
        case AbstractChatListItem::Text:
        case AbstractChatListItem::File:
            in >> package.RoleType >> package.UserChatData.Data;
            break;
        }

        emit received(package);
    }
}

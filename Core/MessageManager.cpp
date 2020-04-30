#include "MessageManager.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <ChatList.h>
#include <LanObject.h>
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
    const auto& chatObj = view->mChatObject;

    out << type;
    out << User::Instance()->getUuid();
    out << User::Instance()->getNickName();
    out << chatObj->getHostAddress();

    // 构建界面使用的item
    ChatItem* item = BuildChatItem(type, data);
    if (!item) {
        emit failed(tr("消息构建失败！"));
        return;
    }
    item->setTime(QDateTime::currentDateTime());
    //item->setSendState(ChatItem::Succeed);
    view->appendItem(item);

    if (type & ChatItem::TCP_Protocol) {
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
    out << chatObj->getRoleType();
    out << data;

    // 发送数据
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, LAN_UDP_PORT);
    } else {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), LAN_UDP_PORT);
    }

    // 保存到本地数据库
    MessageDatabase::Instance()->saveAChatRecord(item, chatObj->getUuid());
}

void MessageManager::sendMessage(IChatObject* chatObj, int type)
{
    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);
    const auto& uuid = User::Instance()->getUuid();

    out << type;
    out << uuid;
    out << User::Instance()->getNickName();
    out << chatObj->getHostAddress();

    switch (type) {
    case AbstractChatListItem::RequestUserInfo:
        out << User::Instance()->getHostAddress();
        break;
    case AbstractChatListItem::ReplyUserInfo:
        out << User::Instance()->getHostAddress();
        // 不要加 break
    case AbstractChatListItem::UserJoin:
        out << QPixmap(AppSettings::AvatarCacheFile(uuid));
        break;
    }

    // 发送数据
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, LAN_UDP_PORT);
    } else {
        mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), LAN_UDP_PORT);
    }
}

void MessageManager::processPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        const int dataSize = int(mUdpSocket->pendingDatagramSize());
        if (dataSize <= 0)
            continue;
        datagram.resize(dataSize);
        mUdpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);

        // 有可能是自己发送的数据
        SChatItemPackage package;
        in >> package.ChatType >> package.UserChatData.Uuid
            >> package.UserChatData.Name >> package.IPAddress;

        switch (package.ChatType) {
        case AbstractChatListItem::Text:
        case AbstractChatListItem::File:
            in >> package.RoleType >> package.UserChatData.Data;
            break;
        case AbstractChatListItem::RequestUserInfo:
            // 如果接收到自己的请求信号就直接返回继续
            if (package.UserChatData.Uuid == User::Instance()->getUuid()) {
                continue;
            }
            in >> package.HostAddress;
            break;
        case AbstractChatListItem::ReplyUserInfo:
            // 如果接收到自己的应答信号就直接返回继续
            if (package.UserChatData.Uuid == User::Instance()->getUuid()) {
                continue;
            }

            in >> package.HostAddress;
            // 不要加break
        case AbstractChatListItem::UserJoin: {
            QPixmap avatar;
            in >> avatar;
            if (!avatar.isNull()) {
                IsDirExists(AppSettings::UserDir() + QStringLiteral("/Avatar"), true);
                avatar.save(AppSettings::AvatarCacheFile(package.UserChatData.Uuid), "JPG", 99);
            }
        } break;
        }

        emit received(package);
    }
}

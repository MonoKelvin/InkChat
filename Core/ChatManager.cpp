#include "ChatManager.h"

#include <ChatView.h>
#include <InkChatApi.h>
#include <MessageDatabase.h>
#include <User.h>

#include <QDataStream>
#include <QDateTime>
#include <QUdpSocket>

ChatManager::ChatManager(QObject* parent)
    : QObject(parent)
    , mPort(LAN_UDP_PORT)
{
    mUdpSocket = new QUdpSocket(this);

    if (mUdpSocket->bind(mPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        connect(mUdpSocket, &QUdpSocket::readyRead, this, &ChatManager::processPendingDatagrams);
        connect(this, &ChatManager::received, MessageDatabase::Instance().data(),
            &MessageDatabase::saveAChatRecord, Qt::QueuedConnection);
    } else {
        mUdpSocket->deleteLater();
    }
}

ChatManager::~ChatManager()
{
    qDebug() << "ChatManager Destroyed";
}

qint64 ChatManager::sendMessage(ChatView* view, IChatObject* chatObj, int type, const QVariant& data)
{
    if (data.isNull() || !data.isValid()) {
        return -1;
    }

    auto time = QDateTime::currentDateTime();
    auto item = ChatView::BuildChatItem(type, time, data);

    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    // 填充数据
    out << User::Instance()->getID();
    out << User::Instance()->getHostAddress();
    out << User::Instance()->getNickName();
    out << type;
    out << data;
    out << time;

    // 发送数据
    qint64 result = -1;
    if (chatObj->getRoleType() == IChatObject::LAN
        || chatObj->getRoleType() == IChatObject::Group) {
        result = mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, mPort);
    } else {
        result = mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), mPort);
    }

    // 显示到视图中并保存到本地数据库
    if (item) {
        view->appendChat(item);
        MessageDatabase::Instance()->saveAChatRecord(item);
        item->setSendState(IChatItem::Succeed);
    }

    return result;
}

void ChatManager::loadChatRecords(ChatView* view, IChatObject* chatObj)
{
    MessageDatabase::Instance()->loadChatItems(view, chatObj->getID());
}

void ChatManager::processPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        const int dataSize = int(mUdpSocket->pendingDatagramSize());
        if (dataSize <= 0) {
            continue;
        }
        datagram.resize(dataSize);
        mUdpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);
        unsigned int senderId = 0;
        QString addr;
        QString name;
        int type;
        QVariant data;
        QDateTime time;
        in >> senderId >> addr >> name >> type >> data >> time;

        IChatItem* item = ChatView::BuildChatItem(type, senderId, time, data);
        if (nullptr == item) {
            continue;
        } else if (item->mChatObject == nullptr) {
            // TODO：加载头像
            item->mChatObject = new IChatObject(item);
            item->mChatObject->setNickName(name);
            item->mChatObject->setRoleType(IChatObject::Stranger);
        } else {
            item->mChatObject->setHostAddress(addr);
        }

        // TODO: 使用消息队列来完成
        emit received(item);
    }
}

#include "MessageManager.h"

#include <ChatView.h>
#include <InkChatApi.h>
#include <MessageDatabase.h>
#include <User.h>

#include <QDataStream>
#include <QDateTime>
#include <QUdpSocket>

MessageManager::MessageManager(QObject* parent)
    : QObject(parent)
    , mPort(LAN_UDP_PORT)
{
    mUdpSocket = new QUdpSocket(this);

    if (mUdpSocket->bind(mPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        connect(mUdpSocket, &QUdpSocket::readyRead, this, &MessageManager::processPendingDatagrams);
        connect(
            this, &MessageManager::received, this, [](IChatItem* item, IChatObject::ERoleType rt) {
                item->mChatObject->getMD5();
                MessageDatabase::Instance()
                    ->saveAChatRecord(item, obj);
            },
            Qt::QueuedConnection);
    } else {
        mUdpSocket->deleteLater();
    }
}

MessageManager::~MessageManager()
{
    qDebug() << "ChatManager Destroyed";
}

qint64 MessageManager::sendMessage(ChatView* view, IChatObject* chatObj, int type, const QVariant& data)
{
    if (data.isNull() || !data.isValid()) {
        return -1;
    }

    auto time = QDateTime::currentDateTime();
    auto item = ChatView::BuildChatItem(type, time, data);

    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    // 填充数据
    out << chatObj->getRoleType();
    out << User::Instance()->getID();
    out << User::Instance()->getHostAddress();
    out << User::Instance()->getNickName();
    out << type;
    out << data;
    out << time;

    // 发送数据
    qint64 result = -1;
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        out << chatObj->getMD5();
        result = mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, mPort);
    } else {
        result = mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress(chatObj->getHostAddress()), mPort);
    }

    // 显示到视图中并保存到本地数据库
    if (item) {
        view->appendChat(item);
        MessageDatabase::Instance()->saveAChatRecord(item, chatObj);
        item->setSendState(IChatItem::Succeed);
    }

    return result;
}

void MessageManager::loadChatRecords(ChatView* view, IChatObject* chatObj)
{
    MessageDatabase::Instance()->loadChatItems(view, chatObj);
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

        // 处理数据
        QDataStream in(&datagram, QIODevice::ReadOnly);
        IChatObject::ERoleType roleType;
        unsigned int senderId = 0;
        QString addr;
        QString name;
        int type;
        QVariant data;
        QDateTime time;
        in >> roleType >> senderId >> addr >> name >> type >> data >> time;

        // 构建到视图
        IChatItem* item = ChatView::BuildChatItem(type, senderId, time, data);
        if (nullptr == item) { // 接收到无效信息
            continue;
        } else if (item->mChatObject == nullptr) { // 如果不是我或者我的好友
            // TODO：加载头像
            item->mChatObject = new IChatObject(item);
            item->mChatObject->setNickName(name);
            item->mChatObject->setRoleType(IChatObject::Stranger);
        }

        item->mChatObject->setHostAddress(addr);

        if (roleType & IChatObject::MultiPerson) {
            QString md5;
            in >> md5;
            item->mChatObject->setMD5(md5);
        }

        // TODO: 使用消息队列来完成
        emit received(item, roleType);
    }
}

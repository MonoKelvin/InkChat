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
    } else {
        emit failed(tr("端口绑定失败，可在“设置”中尝试更改端口！"));
        mUdpSocket->deleteLater();
    }
}

ChatManager::~ChatManager()
{
    qDebug() << "ChatManager Destroyed";
}

qint64 ChatManager::broadcastMessage(int type, const QVariant& data)
{
    if (data.isNull() || !data.isValid()) {
        emit failed(tr("内容不可为空"));
        return -1;
    }

    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    // 只发送自己的ID、主机地址、消息类型和数据
    out << User::Instance()->getID();
    out << User::Instance()->getHostAddress();
    out << type;
    out << data;
    out << QDateTime::currentDateTime();

    return mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, mPort);
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
        int type;
        QString addr;
        QVariant data;
        QDateTime time;
        in >> senderId >> addr >> type >> data >> time;

        emit received(ChatView::BuildChatItem(type, senderId, time, data));
    }
}

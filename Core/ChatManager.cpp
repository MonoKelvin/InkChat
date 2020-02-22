#include "ChatManager.h"

#include <InkChatApi.h>
#include <User.h>

#include <QDataStream>
#include <QUdpSocket>

ChatManager::ChatManager(QObject* parent)
    : QObject(parent)
    , mPort(LAN_UDP_PORT)
{
    mUdpSocket = new QUdpSocket(this);

    if (mUdpSocket->bind(mPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        connect(mUdpSocket, &QUdpSocket::readyRead, this, &ChatManager::processPendingDatagrams);
    } else {
        emit failed(tr("端口绑定失败"));
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
        emit failed(tr("发送的数据为空或无效"));
        return -1;
    }

    QByteArray outData;
    QDataStream out(&outData, QIODevice::WriteOnly);

    // 只发送自己的ID、主机地址和要发送的数据
    out << User::Instance()->getID();
    out << User::Instance()->getHostAddress();
    out << type;
    out << data;

    return mUdpSocket->writeDatagram(outData, outData.length(), QHostAddress::Broadcast, mPort);
}

void ChatManager::processPendingDatagrams()
{
}

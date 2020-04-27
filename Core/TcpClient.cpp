#include "TcpClient.h"

#include <AppSettings.h>
#include <FileChatItem.h>

#include <QFile>
#include <QTcpSocket>

TcpClient::TcpClient(QObject* parent)
    : QObject(parent)
    , mReceivedBytes(0)
    , mTotalBytes(0)
{
    mTcpClient = new QTcpSocket(this);
    connect(mTcpClient, &QTcpSocket::readyRead, this, &TcpClient::readMessage);
}

TcpClient::~TcpClient()
{
    qDebug() << "TcpClient Destroyed:" << this;
    mTcpClient->abort();
    if (mCachedFile && mCachedFile->isOpen()) {
        mCachedFile->close();
    }

    SAFE_DELETE(mCachedFile);
}

void TcpClient::connectToHost(const QString& hostAddr, ChatItem* item)
{
    const QString& name = item->getChatItemData().Message.toString();

    mChatItem = item;
    mCachedFile = new QFile(AppSettings::FileCacheDir() + name);
    if (mCachedFile->exists()) {
        mCachedFile->setFileName(AppSettings::FileCacheDir() + GetDateTimeString() + name);
    }

    mTcpClient->abort();
    mTcpClient->connectToHost(hostAddr, LAN_TCP_PORT);

    mTime.start();
}

void TcpClient::readMessage()
{
    QDataStream in(mTcpClient);

    // 监听00同步信号
    constexpr qint64 syc = qint64(sizeof(qint64) * 2);
    if (mReceivedBytes <= syc) {
        if (mTotalBytes == 0 && mTcpClient->bytesAvailable() >= syc) {
            in >> mTotalBytes;
        } else if (!mCachedFile->open(QFile::WriteOnly)) {
            return;
        }
    }

    if (mReceivedBytes < mTotalBytes) {
        mReceivedBytes += mTcpClient->bytesAvailable();
        mFileData = mTcpClient->readAll();
        mCachedFile->write(mFileData);
        mFileData.resize(0);
    } else {
        deleteLater();
        return;
    }

    switch (mChatItem->getChatType()) {
    case ChatItem::File:
        const auto& fci = static_cast<FileChatItem*>(mChatItem.data());
        const auto& useTime = int(mTime.elapsed() / 1000);
        fci->Speed = mReceivedBytes / useTime;
        fci->Percentage = static_cast<unsigned char>(mReceivedBytes / mTotalBytes);
        break;
    }
}

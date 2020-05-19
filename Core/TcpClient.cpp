#include "TcpClient.h"

#include <AppSettings.h>
#include <FileChatItem.h>

#include <QTcpSocket>

TcpClient::TcpClient(QObject* parent)
    : QObject(parent)
    , mReceivedBytes(0)
    , mTotalBytes(0)
{
    mTcpClient = new QTcpSocket(this);
    connect(mTcpClient, &QTcpSocket::readyRead, this, &TcpClient::readMessage);
    connect(mTcpClient, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpClient::deleteLater);
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
    const QString& name = GetFileNameFromPath(item->getChatItemData().Data.toString());

    mChatItem = item;
    mCachedFile = new QFile(AppSettings::FileCacheDir() + name);
    if (mCachedFile->exists()) {
        mCachedFile->setFileName(AppSettings::FileCacheDir() + GetDateTimeString() + name);
    } else {
        // 必须判断文件夹是否存在，否则传送文件将失败
        IsDirExists(AppSettings::FileCacheDir(), true);
    }

    // 更新文件路径
    static_cast<SUserBaseData*>(item->getData()->data())->Data = mCachedFile->fileName();

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
            if (mTotalBytes == 0) {
                in >> mTotalBytes;
            }
            // 如果需要接收的字节数为0就直接返回
            if (mTotalBytes == 0 || !mCachedFile->open(QFile::WriteOnly)) {
                deleteLater();
                return;
            }
        } else {
            deleteLater();
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
        fci->OtherInfo = GetReadableBytes(qint64(float(mReceivedBytes * 1000) / mTime.elapsed())) + QStringLiteral("/s");
        fci->Percentage = static_cast<unsigned char>(float(mReceivedBytes) / mTotalBytes * 100);
        fci->updataDisplay();
        break;
    }
}

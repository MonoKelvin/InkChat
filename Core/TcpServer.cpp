#include "TcpServer.h"

#include <Configuation.h>
#include <FileChatItem.h>
#include <Utility.h>

#include <QApplication>
#include <QFileDialog>
#include <QTcpServer>
#include <QTcpSocket>

TcpServer::TcpServer(QObject* parent)
    : QObject(parent)
    , mClientConnection(nullptr)
    , mFileToSend(nullptr)
    , mRemainingBytes(0)
    , mTotalBytes(0)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServer::sendMessage);

    mTcpServer->close();
}

TcpServer::~TcpServer()
{
    qDebug() << "TcpServer Destroyed: " << this;

    if (mTcpServer->isListening()) {
        mTcpServer->close();
        if (mFileToSend->isOpen()) {
            mFileToSend->close();
        }
        if (mClientConnection) {
            mClientConnection->abort();
        }
    }

    SAFE_DELETE(mFileToSend);

    // mClientConnection will be automatically deleted when the mTcpServer is destroyed.
    // SAFE_DELETE(mClientConnection);
}

bool TcpServer::setFileToSend(const QString& fileName, ChatItem* chatItem)
{
    mFileToSend = new QFile(fileName);
    if (!mFileToSend->open((QFile::ReadOnly))) {
        deleteLater();
        return false;
    }

    // 开始监听
    if (!mTcpServer->listen(QHostAddress::Any, LAN_TCP_PORT)) {
        deleteLater();
        return false;
    }

    mChatItem = chatItem;

    return true;
}

void TcpServer::sendMessage()
{
    const QString& fn = mChatItem->getChatItemData().Message.toString();
    if (fn.isEmpty()) {
        return;
    }

    mClientConnection = mTcpServer->nextPendingConnection();
    connect(mClientConnection, &QTcpSocket::bytesWritten, this, &TcpServer::updateProgress);

    mTotalBytes = mFileToSend->size();
    QDataStream sendOut(&mFileData, QIODevice::WriteOnly);

    // 开始计时
    mTime.start();

    // 传输00请求同步信号
    sendOut << qint64(0) << qint64(0);

    // 传输总字节数
    //sendOut.device()->seek(0);
    sendOut << mTotalBytes;

    // 计算剩余要发送的数据
    mRemainingBytes = mTotalBytes - mClientConnection->write(mFileData);
    mFileData.resize(0);
}

void TcpServer::updateProgress()
{
    qApp->processEvents();

    // 还有数据要发送
    if (mRemainingBytes > 0) {
        mFileData = mFileToSend->read(qMin(mRemainingBytes, qint64(MAX_FILE_TRANSFER_BLOCK)));
        mRemainingBytes -= mClientConnection->write(mFileData);
        mFileData.resize(0);
    } else {
        // 发送完毕，关闭文件
        deleteLater();
        return;
    }

    switch (mChatItem->getChatType()) {
    case ChatItem::File:
        const auto& fci = static_cast<FileChatItem*>(mChatItem.data());
        const auto& written = mTotalBytes - mRemainingBytes;
        const auto& useTime = int(mTime.elapsed() / 1000);
        fci->Speed = written / useTime;
        fci->Percentage = static_cast<unsigned char>(written / mTotalBytes);
        break;
    }
}

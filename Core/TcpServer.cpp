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
{
    mTcpServer = new QTcpServer(this);

    mTcpPort = LAN_TCP_PORT;
    connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServer::sendMessage);

    mTcpServer->close();
}

TcpServer::~TcpServer()
{
    if (mTcpServer->isListening()) {
        mTcpServer->close();
        if (mFileToSend->isOpen()) {
            mFileToSend->close();
        }
        mClientConnection->abort();
    }

    SAFE_DELETE(mFileToSend);

    // mClientConnection will be automatically deleted when the mTcpServer is destroyed.
    // SAFE_DELETE(mClientConnection);
    qDebug() << "TCP Server Abort: " << this;
}

void TcpServer::setFileToSend(const QString& fileName, ChatItem* chatItem)
{
    if (!isFileExists(fileName)) {
        emit failed(tr("文件不存在"));
        return;
    }

    mChatItem = chatItem;
    mChatItem->setData(fileName);

    if (!mTcpServer->listen(QHostAddress::Any, mTcpPort)) //开始监听
    {
        emit failed(tr("文件发送失败，请重试！"));
        this->deleteLater();
        return;
    }
}

void TcpServer::sendMessage()
{
    const QString& fn = mChatItem->getData().toString();
    if (fn.isEmpty()) {
        emit failed(tr("没有文件要发送"));
        return;
    }

    mClientConnection = mTcpServer->nextPendingConnection();
    connect(mClientConnection, &QTcpSocket::bytesWritten, this, &TcpServer::updateProgress);

    mFileToSend = new QFile(fn);
    if (!mFileToSend->open((QFile::ReadOnly))) {
        emit failed(tr("应用程序无法读取文件%1\n错误原因：%2").arg(fn).arg(mFileToSend->errorString()));
        return;
    }

    mTotalBytes = mFileToSend->size();
    QDataStream sendOut(&mFileData, QIODevice::WriteOnly);

    // 开始计时
    mTime.start();

    // 先传输文件名，一定不能包括路径
    sendOut << qint64(0) << qint64(0) << getFileNameFromPath(fn);
    mTotalBytes += mFileData.size();

    // 传输总字节数
    sendOut.device()->seek(0);
    sendOut << mTotalBytes << (mFileData.size() - qint64(sizeof(qint64) * 2));

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
        fci->Speed = getBytesForHumanReadable(written / useTime) + QStringLiteral("/s");
        fci->Percentage = static_cast<unsigned char>(written / mTotalBytes);
        break;
    }
}

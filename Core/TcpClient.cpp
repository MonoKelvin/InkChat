#include "TcpClient.h"

#include <AppSettings.h>

#include <QFile>
#include <QTcpSocket>

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    mTcpClient = new QTcpSocket(this);

    mTcpPort = LAN_TCP_PORT;

    connect(mTcpClient, &QTcpSocket::readyRead, this, &TcpClient::readMessage);
    connect(mTcpClient, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
        [this] {
            emit failed(mTcpClient->errorString());
        });
}

TcpClient::~TcpClient()
{
    closeTcpClient();
}

void TcpClient::setCacheFileName(const QString& name)
{
    QString fileName = AppSettings::FileCacheDir() + name;
    if (QFile(fileName).exists()) {
        fileName = AppSettings::FileCacheDir() + getDateTimeString() + name;
        isFileExists(fileName, true);
    }

    if (!mCacheFile) {
        mCacheFile = new QFile(fileName);
    } else {
        mCacheFile->setFileName(fileName);
    }
}

void TcpClient::closeTcpClient()
{
    mTcpClient->abort();
    if (mCacheFile->isOpen()) {
        mCacheFile->close();
    }
}

void TcpClient::newConnect()
{
    mTcpClient->abort();
    mTcpClient->connectToHost(mHostAddress, mTcpPort);
    mTime.start();
}

void TcpClient::readMessage()
{
}

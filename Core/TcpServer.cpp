#include "TcpServer.h"

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
}

void TcpServer::setFileToSend()
{
    const auto& fileName = QFileDialog::getOpenFileName();
    if (!fileName.isEmpty()) {
        mData.FileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    }
}

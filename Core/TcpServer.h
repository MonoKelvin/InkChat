#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <ChatItem.h>
#include <Configuation.h>

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QElapsedTimer>
#else
#include <QTime>
#endif

class QFile;
class QTcpServer;
class QTcpSocket;

class TcpServer : public QObject {
    Q_OBJECT
public:
    explicit TcpServer(QObject* parent = nullptr);
    ~TcpServer();

    inline void setHostAddress(const QString& hostAddr) noexcept
    {
        mHostAddress = hostAddr;
        newConnect();
    }

    const SChatItemData& getChatItemData() const

    {
        return mData;
    }

Q_SIGNALS:
    void failed(const QString& msg);

private Q_SLOTS:
    /**
     * @brief 设置要发送的文件
     * @note 该方法是打开文件对话框来获取文件
     */
    void setFileToSend();

    /**
     * @brief 关闭TCP客户端连接
     */
    void closeTcpServer();

    /**
     * @brief 新建连接
     */
    void newConnect();

    /**
     * @brief 读消息
     */
    void readMessage();

private Q_SLOTS:
    void sendMessage();
    void updateClientProgress(qint64 numBytes);

private:
    /** TCP服务端 */
    QTcpServer* mTcpServer;

    /** TCP服务端与客户端的连接 */
    QTcpSocket* mTlientConnection;

    /** 主机地址 */
    QString mHostAddress;

    /** TCP 端口号 */
    Port mTcpPort;

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QElapsedTimer mTime;
#else
    /** 时间戳 */
    QTime mTime;
#endif

    /** 指向的缓存文件 */
    QFile* mCacheFile;

    /** 发送的数据 */
    STcpFileData mData;
};

#endif // TCPSERVER_H

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <Configuation.h>

#include <QPointer>
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QElapsedTimer>
#else
#include <QTime>
#endif

class QFile;
class ChatItem;
class QTcpServer;
class QTcpSocket;

/**
 * @brief TCP 服务器，专门用于传输文件，可以是任意普通文件，暂不支持文件夹的传输。
 */
class TcpServer : public QObject {
    Q_OBJECT
public:
    explicit TcpServer(QObject* parent = nullptr);
    ~TcpServer();

    /**
     * @brief 设置要发送的文件
     * @param fileName 文件名，包括路径
     * @param chatItem 指向的聊天控件地址，用于更新其相关数据
     * @param 如果文件路径正确并且成功建立TCP连接就返回true，否则返回false
     */
    bool setFileToSend(const QString& fileName, ChatItem* chatItem);

private Q_SLOTS:

    /**
     * @brief 发送数据
     */
    void sendMessage(void);

    /**
     * @brief 更新进程
     */
    void updateProgress();

private:
    /** TCP服务端 */
    QTcpServer* mTcpServer;

    /** TCP服务端与客户端的连接 */
    QTcpSocket* mClientConnection;

    /** 要发送的文件 */
    QFile* mFileToSend;

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QElapsedTimer mTime;
#else
    /** 时间戳 */
    QTime mTime;
#endif

    /** 文件要传输的实际数据 */
    QByteArray mFileData;

    /* 剩余还要传输字节 */
    qint64 mRemainingBytes;

    /** 总共要传输的字节 */
    qint64 mTotalBytes;

    /** 保存指向的聊天信息控件 */
    QPointer<ChatItem> mChatItem;
};

#endif // TCPSERVER_H

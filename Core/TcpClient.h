#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QPointer>

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QElapsedTimer>
#else
#include <QTime>
#endif

class ChatItem;
class QFile;
class QTcpSocket;
struct SUserBaseData;

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject* parent = nullptr);
    ~TcpClient();

    /**
     * @brief 连接到主机以接收文件
     * @param hostAddr 主机IPv4地址
     * @param item 指向的聊天视图中的聊天项
     * @note 连接到主机后，会自动接收文件并缓存到目录：{app-path}/Data/Files/，如果文件
     * 存在则会创建一个新的文件名，该文件名前会添上时间串。
     */
    void connectToHost(const QString& hostAddr, ChatItem* item);

private Q_SLOTS:
    /**
     * @brief 读消息
     */
    void readMessage();

private:
    /** TCP客户端 */
    QTcpSocket* mTcpClient;

    /** 指向的缓存文件 */
    QFile* mCachedFile;

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QElapsedTimer mTime;
#else
    /** 时间戳 */
    QTime mTime;
#endif

    /** 文件要传输的实际数据 */
    QByteArray mFileData;

    /* 接收到的字节 */
    qint64 mReceivedBytes;

    /** 总共要传输的字节 */
    qint64 mTotalBytes;

    /** 接收的数据 */
    QPointer<ChatItem> mChatItem;
};

#endif // TCPCLIENT_H

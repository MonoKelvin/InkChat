#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <ChatItem.h>
#include <Configuation.h>

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QElapsedTimer>
#else
#include <QTime>
#endif

class QFile;
class QTcpSocket;

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject* parent = nullptr);
    ~TcpClient();

    inline void setHostAddress(const QString& hostAddr) noexcept
    {
        mHostAddress = hostAddr;
        newConnect();
    }

    /**
     * @brief 设置缓存的文件名，不包括路径，只是文件名和后缀
     * @param name 文件名，只含名字和后缀
     * @note 文件缓存路径为 {app-path}/Data/Files/ ，如果文件存在则会创建一个新的文件，该
     * 文件名前会添上时间串。
     */
    void setCacheFileName(const QString& name);

    const SChatItemData& getChatItemData() const
    {
        return mData;
    }

Q_SIGNALS:
    void failed(const QString& msg);

private Q_SLOTS:
    /**
     * @brief 关闭TCP客户端连接
     */
    void closeTcpClient();

    /**
     * @brief 新建连接
     */
    void newConnect();

    /**
     * @brief 读消息
     */
    void readMessage();

private:
    /** TCP客户端 */
    QTcpSocket* mTcpClient;

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

    /** 接收的数据 */
    STcpFileData mData;
};

#endif // TCPCLIENT_H

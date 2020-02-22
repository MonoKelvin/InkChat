#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QSharedPointer>

/** 定义端口号类型 */
typedef quint16 Port;

/** 定义默认局域网UDP协议端口号为2020 */
#define LAN_UDP_PORT 2020

class IChatItem;
class QUdpSocket;
//class TcpServer;

/**
 * @brief 聊天消息管理器，用于收发聊天消息、收发文件、图片等
 * @note 作为接收管理器时：每当收到数据时就数据递交给聊天ChatView，聊天视图解包创建可视化聊天
 * 控件，之后再将数据交付给MessageDatabase进行本地数据存储；
 * @note 作为发送管理器时：从聊天视图获取的发送消息交由ChatView处理，再由MessageDatabase进
 * 行存储，最后递交给ChatManager打包发送。
 */
class ChatManager : public QObject
{
    Q_OBJECT

    explicit ChatManager(QObject* parent = nullptr);

public:
    ~ChatManager(void);

    static QSharedPointer<ChatManager> Instance()
    {
        static auto instance = QSharedPointer<ChatManager>(new ChatManager);
        return instance;
    }

    /**
     * @brief 以广播形式发送我的消息
     * @return type 消息类型，根据该类型可以按照不同的解析方式来解析data生成聊天记录
     * @param data 要发送的数据
     * @return qint64 返回发送成功的字节数，如果失败则返回-1
     * @note 该消息主要针对UDP广播协议
     */
    qint64 broadcastMessage(int type, const QVariant& data);

private Q_SLOTS:
    /**
     * @brief 处理正在等待的UDP数据报文
     */
    void processPendingDatagrams();

Q_SIGNALS:
    void failed(const QString& msg);

private:
    /** 
     * @brief 用于消息收发的UDP套接字
     * 该套接字主要用于局域网：用来收发普通文本、富文本消息，用户离线、上线通知
     */
    QUdpSocket* mUdpSocket;

    /**
     * @brief TCP服务器
     * 用于发送文件、图片、视频等
     */
    //TcpServer* mTcpServer;    // TODO: add TcpServer class

    /** 端口号，用于局域网聊天 */
    Port mPort;
};

#endif // CHATMANAGER_H

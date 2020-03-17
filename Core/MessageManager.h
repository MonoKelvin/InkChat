#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <IChatObject.h>

#include <QSharedPointer>

/** 定义端口号类型 */
typedef quint16 Port;

/** 定义默认局域网UDP协议端口号为2020 */
#define LAN_UDP_PORT 2020

class IChatItem;
class QUdpSocket;
class ChatView;
//class TcpServer;

/**
 * @brief 聊天消息管理器，用于收发聊天消息、收发文件、图片等
 * @note 作为接收管理器时：每当收到数据时就数据递交给MessageDatabase进行本地数据存储（可能
 * 存储失败），之后再交付给ChatView解包创建可视化聊天控件（可能创建失败）；
 * @note 作为发送管理器时：从聊天视图获取的发送消息交由ChatView处理，再由MessageDatabase进
 * 行存储（可能存储失败），最后递交给ChatManager打包发送（可能发送失败）。
 */
class MessageManager : public QObject {
    Q_OBJECT

    explicit MessageManager(QObject* parent = nullptr);

public:
    ~MessageManager(void);

    static QSharedPointer<MessageManager> Instance()
    {
        static auto instance = QSharedPointer<MessageManager>(new MessageManager);
        return instance;
    }

    /**
     * @brief 发送消息
     * @param view 聊天视图。消息将被发送到UI界面呈现
     * @param chatObj 聊天对象
     * @param type 消息类型
     * @param data 要发送的消息数据
     * @return qint64 返回发送成功的字节数，如果失败则返回-1
     */
    qint64 sendMessage(ChatView* view, IChatObject* chatObj, int type, const QVariant& data);

    /**
     * @brief 加载聊天记录
     * @param view 聊天视图
     * @param chatObj 聊天对象
     */
    void loadChatRecords(ChatView* view, IChatObject* chatObj);

private Q_SLOTS:
    /**
     * @brief 处理正在等待的UDP数据报文
     * @note 该方法会在后台进行，所有消息将被推送到消息队列进行派发。
     * @note 如果发送来的消息可被成功解析，该方法会在后台自动选择或创建一个消息项MessageItem；
     * 如果列表中无该消息，则创建并插入到消息列表MessageList，同时更新未读消息数，如果有排序
     * 算法则应用算法更新列表。而如果列表中存在该项，则直接更新消息数。如果用户正打开该项进行聊
     * 天，则直接发送至页面。
     */
    void processPendingDatagrams();

    /**
     * @brief 更新消息，通常为接收到用户发来的消息
     * @param userId 更新指定聊天对象的消息
     * @note 如果指定的id不在列表中，则会创建一个新的项；如果打开的项所对应的id为chatObjId，
     * 则会更新聊天视图的内容
     */
    //void update(unsigned int chatObjId);

Q_SIGNALS:

    /**
     * @brief 信号：接收到数据
     * @param IChatObject::ERoleType 来自与聊天对象的实例的角色类型
     * @param IChatItem* 接收到数据并封装好的聊天项，可能为nullptr
     * @note 通常该信号会关联多个槽函数，即分发消息。在视图中接收到信号后判断该聊天消息是否
     * 为自己需要的，如果需要则接收处理，否则可以抛弃。
     */
    void received(IChatObject::ERoleType, IChatItem*);

private:
    /** 
     * @brief 用于消息收发的UDP套接字
     * 该套接字主要用于局域网收发普通文本、富文本消息，用户离线、上线通知等
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

#endif // MESSAGEMANAGER_H

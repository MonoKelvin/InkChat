#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <AbstractChatListItem.h>

#include <QSharedPointer>

class ChatItem;
class QUdpSocket;
class ChatList;
class MessageItem;
class IChatObject;
struct SUserBaseData;
struct SChatItemPackage;

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
     * @brief 构建一个聊天项
     * @param chatType
     * @param userData
     * @return
     * @note 构建好的item不会发送到视图、也不会保存到数据库
     */
    static ChatItem* BuildChatItem(int chatType, const SUserBaseData& userData);

    /**
     * @brief 构建一个由我发送的聊天项
     * @param chatType
     * @param data
     * @return 
     * @note 构建好的item不会发送到视图、也不会保存到数据库
     */
    static ChatItem* BuildChatItem(int chatType, const QVariant& data);

    /**
     * @brief BuildChatListItem
     * @param type
     * @param data
     * @return 
     */
    static AbstractChatListItem* BuildChatListItem(int chatType, const QVariant& data);

    /**
     * @brief 注册一个聊天类。这样在就可使用自定义的聊天类发送或接收消息
     */
    template <typename T>
    inline static void RegisterChatItemClass()
    {
        const auto& metaClass = T::staticMetaObject;

        Q_ASSERT(metaClass.inherits(&AbstractChatListItem::staticMetaObject));

        if (mRegistryChatClasses.contains(T::ChatType)) {
            qWarning("AbstractChatListItem类型已存在，原类型将会被覆盖");
        }

        qRegisterMetaType<T>(metaClass.className());
        mRegistryChatClasses.insert(T::ChatType, metaClass.className());
    }

    inline static const QString GetUserBehavior(int type, const QString& name) noexcept
    {
        if (type == AbstractChatListItem::UserJoin) {
            return QObject::tr("用户 %1 加入").arg(name);
        }
        if (type == AbstractChatListItem::UserLeft) {
            return QObject::tr("用户 %1 离开").arg(name);
        }

        return QString();
    }

    /**
     * @brief 发送消息
     * @param view 聊天视图。消息将被发送到UI界面呈现
     * @param type 消息类型
     * @param data 要发送的消息数据
     * @note 发送失败消息通过 @see failed 给出
     */
    void sendMessage(ChatList* view, int type, const QVariant& data = QVariant());

    void sendUserBehavior(const QString& addr, int type = AbstractChatListItem::UserJoin);

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
     * @brief 信号：接收到聊天的数据
     * @param package 发送方的发来的数据包。
     */
    void received(const SChatItemPackage& package);

    void failed(const QString& msg);

private:
    /** 
     * @brief 用于消息收发的UDP套接字
     * 该套接字主要用于局域网收发普通文本、富文本消息，用户离线、上线通知等
     */
    QUdpSocket* mUdpSocket;

    /**
     * @brief 注册的聊天类容器
     * @type int 表示类的 ChatType
     * @type QByteArray 表示类的名称，即在qml中可以访问的类名
     */
    static QHash<int, QByteArray> mRegistryChatClasses;
};

#endif // MESSAGEMANAGER_H

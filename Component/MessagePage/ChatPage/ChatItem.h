#ifndef CHATITEM_H
#define CHATITEM_H

#include <AbstractChatListItem.h>
#include <IChatObject.h>

#include <QAbstractItemDelegate>
#include <QDateTime>

/**
 * @brief UDP 传输时使用的 ChatItem 数据包
 */
struct SChatItemPackage {
    SUserBaseData UserChatData; // 传输时聊天对象对应的用户数据，单用户时聊天对象即为用户
    IChatObject::ERoleType RoleType; // 传输时的聊天对象
    QString IPAddress; // 聊天对象IP地址，如果是单用户则该值等于HostAddress
    QString HostAddress; // 用户主机地址
    int ChatType; // 聊天类型

    SChatItemPackage() = default;
};

/**
 * @brief 聊天消息基类，可以实现自定义聊天内容。比如消息气泡，文件、图片、视频等聊天内容，其
 * 数据内容是用户有关的，即会通过网络发送接收
 * @see NotificationItem
 */
class ChatItem : public AbstractChatListItem {
    Q_OBJECT

    CHAT_ITEM_OBJECT(ChatItem, 0)

public:
    explicit ChatItem(QObject* parent = nullptr);
    ChatItem(const ChatItem& item);
    virtual ~ChatItem() override;

    /**
     * @brief 聊天消息发送状态枚举
     */
    enum ESendState {
        Sending, // 正在发送
        Succeed, // 发送成功
        Failed, // 发送失败
        Withdraw, // 撤回
        Rejected, // 发送被拒绝，一般被拒绝的是文件，其他内容谨慎使用
    };

    inline ESendState getSendState(void) const { return mSendState; }
    void setSendState(const ESendState &sendState);

    //inline int getChatId(void) const { return mChatId; }
    //inline void setChatId(int chatId) { mChatId = chatId; }

    inline const QDateTime getTime(void) const { return mTime; }
    inline void setTime(const QDateTime &time) { mTime = time; }

    /**
     * @brief 获得处理后的消息时间
     * @return 返回处理后的消息字符串，默认处理函数为 @see GetMessageTime
     * @see getTime
     */
    virtual const QString getMessageTime(void);

    /**
     * @brief 绘制内容。针对于QWidget作为界面开发使用
     * @param painter 画笔，需要使用save、restore来暂存设置
     * @param availableRect 可用矩形，即聊天内容可以使用范围，其中可以忽略高度参数
     * @return const QRect 内容矩形
     * @warning 使用availableRect时请注意使用相对位置，比如在矩形中绘制(0,0)点，即左上角，
     * 务必使用(availableRect.left(), availableRect.top())替代，否则视图中无法正常显示
     */
    virtual void paintContent(QPainter* painter, const QRect& availableRect) override = 0;

    /**
     * @brief 更新内容区域尺寸
     * @param const QStyleOptionViewItem& 样式
     * @return 返回内容区域尺寸
     */
    virtual void updateContentSize(const QStyleOptionViewItem&) override = 0;

    inline virtual void setChatItemData(const SUserBaseData& data) noexcept
    {
        mData.setValue(data);
    }

    const SUserBaseData getChatItemData() const noexcept;

Q_SIGNALS:
    void sendStateChanged();

protected:
    /** 发送状态 */
    ESendState mSendState;

    /**
     * @brief 消息ID
     * @note 消息ID是用于排列消息顺序的，即最大ID的消息气泡将在聊天视图的最底端
     */
    //int mChatId;

    /**
     * @brief 消息发送或接收的时间
     * @note 一般消息发送会有延时，那么该值表示的是：在接收方为完整接收的时间，在发送方为消息
     * 发送成功的时间。此外该，每次发送状态 @see mSendState 改变时，该值将会重置为当前时间。
     * @note 该时间默认经过处理后才显示到UI界面上的，处理函数为 @see GetMessageTime
     */
    QDateTime mTime;
};

/**
 * @brief 聊天消息代理
 */
class ChatItemDelegate : public QAbstractItemDelegate {
public:
    explicit ChatItemDelegate(QObject* parent = nullptr)
        : QAbstractItemDelegate(parent)
    {
    }

    enum { ChatItemType = Qt::UserRole + 101 };

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // CHATITEM_H

#ifndef ICHATITEM_H
#define ICHATITEM_H

#include <IChatObject.h>

#include <QAbstractItemDelegate>
#include <QDateTime>
#include <QPointer>

/** 继承自IChatItem的子类必须使用该宏，否则无法注册到聊天视图中 */
#define CHATITEM_OBJECT(_ClassName_, _ChatType_)                       \
public:                                                                \
    Q_INVOKABLE explicit _ClassName_(QObject* parent = nullptr);       \
    unsigned int getChatType(void) const override { return ChatType; } \
                                                                       \
private:                                                               \
    friend class ChatItemDelegate;                                     \
    friend class ChatList;                                             \
    enum { ChatType = _ChatType_ };

class QPainter;

/**
 * @brief 聊天消息基类，可以实现自定义聊天内容。比如消息气泡，文件、图片、视频等聊天内容
 */
class IChatItem : public QObject
{
    Q_OBJECT

    friend class ChatList;
    friend class MessageManager;
    friend class MessageDatabase;

public:
    explicit IChatItem(QObject* parent = nullptr);
    IChatItem(const IChatItem& item);
    virtual ~IChatItem();

    /**
     * @brief 系统定义的基本聊天内容的类型
     * @note 用户不可重定义
     */
    enum EBaseChatType {
        Text = 1, // 普通文本
        RichText, // 富文本
        File, // 文件
        Image, // 图片
        MultiImages, // 多张图片
        Video, // 视频
        Audio, // 音频

        Notification, // 通知
        UserEntry, // 用户进入局域网
        UserLeft, // 用户离开局域网
    };

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

    /**
     * @brief 获得聊天类型
     * @return 聊天类型，该返回值尽量使用先对枚举ChatType赋值，再返回
     */
    virtual unsigned int getChatType() const = 0;

    inline ESendState getSendState(void) const { return mSendState; }
    void setSendState(const ESendState &sendState);

    /**
     * @brief 获得消息的发送者
     * @note 默认角色类型为ERoleType::Me的发送对象是在聊天视图@see ChatView 的右边显示；
     * 其他类型都是在左边显示。
     */
    inline IChatObject *getSender(void) const
    {
        return mChatObject.data();
    }

    inline int getChatId(void) const { return mChatId; }
    inline void setChatId(int chatId) { mChatId = chatId; }

    inline const QDateTime getTime(void) const { return mTime; }
    inline void setTime(const QDateTime &time) { mTime = time; }

    /**
     * @brief 获得处理后的消息时间
     * @return 返回处理后的消息字符串，默认处理函数为 @see GetMessageTime
     * @see getTime
     */
    virtual const QString getMessageTime(void);

    /**
     * @brief 获得qml组件文件路径。聊天视图中如果发送该类型的聊天控件，则使用的qml组件文件为其返回值
     * @return 返回qml组件文件。
     * @note 该组件文件必须包含在qrc文件中
     */
    //virtual const QString qmlFile() = 0;

    /**
     * @brief 绘制内容。针对于QWidget作为界面开发使用
     * @param painter 画笔，需要使用save、restore来暂存设置
     * @param availableRect 可用矩形，即聊天内容可以使用范围，其中可以忽略高度参数
     * @return const QRect 内容矩形
     * @warning 使用availableRect时请注意使用相对位置，比如在矩形中绘制(0,0)点，即左上角，
     * 务必使用(availableRect.left(), availableRect.top())替代，否则视图中无法正常显示
     */
    virtual void paintContent(QPainter* painter, const QRect& availableRect) = 0;

    /**
     * @brief 更新内容区域尺寸
     * @param const QStyleOptionViewItem& 样式
     * @return 返回内容区域尺寸
     */
    virtual void updateContentSize(const QStyleOptionViewItem&) {}

    /**
     * @brief 获得相对与聊天视图的区域大小
     * @return const QSize 内容大小
     */
    Q_DECL_CONSTEXPR const QSize getContentSize(void) const noexcept
    {
        return mContentSize;
    }

    /**
     * @brief 是否是自身绘制方法
     * @return 是否自身绘制，如果返回true，则代理中不会显示头像、姓名等，否则只需绘制消息内容
     */
    virtual bool selfPaint(void)
    {
        return false;
    }

    /**
     * @brief 接收数据抽象方法，旨在从网络信道接收数据，解析后给成员赋值。
     * @param data 传入的数据引用，即由底层分发数据，具体实现细节由该方法决定。
     * @warning 内部数据流必须和 sendData 一致，否则容易出错。
     */
    //virtual void receiveData(QByteArray& data) = 0;

    /**
     * @brief 发送数据抽象方法，旨在从网络信道发送数据，封装后发送给底层。
     * @param data 保存发送到网络的数据流，及内部实现数据封装，具体发送细节由底层决定
     * @warning 内部数据流必须和 receiveData 一致，否则容易出错。
     */
    //virtual void sendData(QByteArray& data) = 0;

    /**
     * @brief 解析数据方法
     * @param data 来自外界的数据，进行内部处理。
     * @note 一般用于将接收、发送的消息解析后保存并显示在界面
     */
    virtual void praseData(const QVariant& data) = 0;

    /**
     * @brief 获取数据
     * @return const QVariant 主要用于保存到数据库
     */
    virtual const QVariant getData(void) = 0;

protected:
    /**
     * @brief 内容尺寸，子类需要在 @see updateContentSize 方法内更新该值
     */
    QSize mContentSize;

Q_SIGNALS:
    void sendStateChanged();

protected:
    /**
     * @brief 聊天项类型
     * @note 子类必须设置ChatType为不同值来作为聊天项角色标记，若有多个自定义类，需要为每一
     * 个ChatType重新定义不同的值。且所有值必须大于 @see Qt::UserRole
     */
    enum { ChatType = Qt::UserRole };

    /** 发送状态 */
    ESendState mSendState;

    /**
     * @brief 消息ID
     * @note 消息ID是用于排列消息顺序的，即最大ID的消息气泡将在聊天视图的最底端
     */
    int mChatId;

    /**
     * @brief 消息发送或接收的时间
     * @note 一般消息发送会有延时，那么该值表示的是：在接收方为完整接收的时间，在发送方为消息
     * 发送成功的时间。此外该，每次发送状态 @see mSendState 改变时，该值将会重置为当前时间。
     * @note 该时间默认经过处理后才显示到UI界面上的，处理函数为 @see GetMessageTime
     */
    QDateTime mTime;

    /** 发送聊天消息的对象 */
    QPointer<IChatObject> mChatObject;
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

#endif // ICHATITEM_H

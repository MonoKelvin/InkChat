#ifndef ICHATITEM_H
#define ICHATITEM_H

#include <IChatObject.h>

#include <QDateTime>
#include <QSharedPointer>

/**
 * @brief 聊天消息基类，可以实现自定义聊天内容。比如消息气泡，文件、图片、视频等聊天内容
 */
class IChatItem : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(IChatItem)

    friend class MessageDatabase;
    friend class ChatView;

private:
    Q_PROPERTY(ESendState sendState READ getSendState WRITE setSendState NOTIFY sendStateChanged)
    Q_PROPERTY(unsigned int id READ getChatId CONSTANT)
    Q_PROPERTY(QString time READ getMessageTime CONSTANT)
    Q_PROPERTY(IChatObject* sender READ getSender CONSTANT)

public:
    explicit IChatItem(QObject* parent = nullptr);
    virtual ~IChatItem();

    /**
     * @brief 角色类型枚举。子类必须重新定义不一样的值
     */
    enum ERole {
        ChatItemRole = Qt::UserRole + 1
    };

    /**
     * @brief 聊天消息发送状态枚举
     */
    enum ESendState
    {
        Sending,  // 正在发送
        Succeed,  // 发送成功
        Failed,   // 发送失败
        Withdraw, // 撤回
        Rejected, // 发送被拒绝，一般被拒绝的是文件，其他内容谨慎使用
    };
    Q_ENUM(ESendState)

    inline ESendState getSendState(void) const { return mSendState; }
    void setSendState(const ESendState& sendState);

    /**
     * @brief 获得qml组件文件路径。聊天视图中如果发送该类型的聊天控件，则使用的qml组件文件为其返回值
     * @return 返回qml组件文件。
     * @note 该组件文件必须包含在qrc文件中
     */
    Q_INVOKABLE virtual const QString qmlFile() = 0;

    /**
     * @brief 获得消息的发送者
     * @note 默认角色类型为ERoleType::Me的发送对象是在聊天视图@see ChatView 的右边显示；
     * 其他类型都是在左边显示。
     */
    inline IChatObject* getSender(void) const
    {
        return mChatObject.data();
    }

    inline unsigned int getChatId(void) const { return mChatId; }
    inline void setChatId(unsigned int chatId) { mChatId = chatId; }

    /**
     * @brief 获得处理后的消息时间
     * @return 返回处理后的消息字符串，默认处理函数为 @see GetMessageTime
     * @see getTime
     */
    virtual const QString getMessageTime(void);
    inline const QDateTime getTime(void) const { return mTime; }
    inline void setTime(const QDateTime& time) { mTime = time; }

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
     * @brief 打包方法。用于保存在数据库中
     * @param data 将数据封装到容器输出。第一位是数据名称，第二位参数是数据
     * @note 传出的数据会递交给数据库处理或发送消息，逆过程要和 @see unpackage 一致
     */
    virtual void package(QVariantMap& data) = 0;

    /**
     * @brief 解包数据方法
     * @param data 来自外界的数据，进行内部处理。第一位是数据名称，第二位参数是数据
     * @note 一般用于从数据库中回复和接收消息，逆过程要和 @see package 一致
     */
    virtual void unpackage(QVariantMap& data) = 0;

Q_SIGNALS:
    void sendStateChanged();

protected:
    /** 发送状态 */
    ESendState mSendState;

    /**
     * @brief 消息ID
     * @note 消息ID是用于排列消息顺序的，即最大ID的消息气泡将在聊天视图的最底端
     */
    unsigned int mChatId;

    /**
     * @brief 消息发送或接收的时间
     * @note 一般消息发送会有延时，那么该值表示的是：在接收方为完整接收的时间，在发送方为消息
     * 发送成功的时间。此外该，每次发送状态 @see mSendState 改变时，该值将会重置为当前时间。
     * @note 该时间默认经过处理后才显示到UI界面上的，处理函数为 @see GetMessageTime
     */
    QDateTime mTime;

    /** 发送聊天消息的对象 */
    QSharedPointer<IChatObject> mChatObject;
};

#endif // ICHATITEM_H

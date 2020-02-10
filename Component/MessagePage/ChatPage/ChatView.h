#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QObject>

#define CHATVIEW_INITIALIZA                                            \
    {                                                                  \
        qmlRegisterType<ChatView>("ChatView", 1, 0, "ChatPageManage"); \
    }

class User;

class ChatView : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ChatView)

public:
    ChatView() = default;

    ~ChatView() = default;

    /**
     * @brief 发送聊天消息的发送方枚举
     */
    enum class ESender
    {
        System, // 系统
        Me, // 自己
        Ta, // ta（他或她，一个人）
        Other, // 其他人（针对群聊天）
    };
    Q_ENUM(ESender)

    /**
     * @brief 聊天消息发送状态枚举
     */
    enum class ESendState
    {
        Sending, // 正在发送
        Succeed, // 发送成功
        Failed, // 发送失败
        Withdraw, // 撤回
    };
    Q_ENUM(ESendState)

    enum class EMessageType
    {
        Message,
        Time,
        File,
        Image,
        MutiImages,
        Video,
        Audio
    };
    Q_ENUM(EMessageType)

//Q_SIGNALS:
//    void sendMessage(QSharedPointer<User> user, const QString &message);

};

#endif // CHATVIEW_H

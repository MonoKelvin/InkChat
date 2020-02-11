#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QObject>

#define MESSAGELIST_INITIALIZA                                                        \
    {                                                                                 \
        qmlRegisterType<MessageList>("MessageListManage", 1, 0, "MessageListManage"); \
    }

class MessageList : public QObject {
    Q_OBJECT
public:
    /**
     * @brief 消息保存或加载时的过滤器
     * @todo Next Version
     */
    enum EMessageFilter {
        FriendMessages = 0x0001,
        LANMessage = 0x0002,
        GroupMessages = 0x0004,
        NoFilter = FriendMessages | LANMessage | GroupMessages,
    };
    Q_ENUM(EMessageFilter)

public slots:
    bool refresh();
    bool load(const QString& crFolder /*, EMessageFilter filter = NoFilter*/);
    bool save(const QString& crFileName /*, EMessageFilter filter = NoFilter*/);

Q_SIGNALS:
    void saved();
    void loaded();
    void failed(const QString&);

private:
    /**
     * @brief 消息记录文件夹
     */
    QString mCrFolder;
};

#endif // MESSAGELIST_H

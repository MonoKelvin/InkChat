#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QWidget>

class QLabel;
class Avatar;

class MessageItem : public QWidget
{
    Q_OBJECT

    friend class AdvancedList;
public:
    explicit MessageItem(unsigned int id, QWidget *parent = nullptr);

    MessageItem(unsigned int id,
                const QPixmap &avatar,
                const QString &name,
                const QString &message = QString(),
                QWidget *parent = nullptr);

signals:
    void onRead(unsigned int id);

protected slots:
    void readMessage();
    void unreadMessage();

private:
    void init();

private:
    /** 头向控件 */
    Avatar *mAvatar;

    /** 显示的名称，如果是用户则为昵称或备注名，如果是群则为群名 */
    QString mName;

    /** 显示的消息概要，超过item长度以...的形式结尾 */
    QString mMessage;

    /** 未读消息数量 */
    unsigned int mMessageNumber;

    /** 这条消息在数据库中的ID */
    unsigned int mID;

    /** 是否已读 */
    bool mIsRead;
};

#endif // MESSAGEITEM_H

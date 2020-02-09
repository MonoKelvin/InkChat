#ifndef MYFRIEND_H
#define MYFRIEND_H

#include <QObject>

class MyFriend : public QObject
{
    Q_OBJECT
    friend class User;

public:
    explicit MyFriend(QObject* parent = nullptr);

    void fromJson(const QJsonObject& json);
    QJsonObject toJson(void);

private:
    unsigned int UID;
    bool IsTop;
    char Gender;
    QString Remark;
    QString Signature;
    QString Thumb;
    QString Subgroup;
    QString HostAddress;
};

#endif // MYFRIEND_H

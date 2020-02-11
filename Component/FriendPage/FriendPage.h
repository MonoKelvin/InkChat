#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include <QObject>

#define FRIENDPAGE_INITIALIZA                                                      \
    {                                                                              \
        qmlRegisterType<FriendPage>("FriendPageManage", 1, 0, "FriendPageManage"); \
    }

class User;

class FriendPage : public QObject
{
    Q_OBJECT
public:
    explicit FriendPage(QObject* parent = nullptr);
    ~FriendPage();

public slots:
    void requestFriendsData(User* user);

Q_SIGNALS:
    void failed(const QString&);
};

#endif // FRIENDPAGE_H

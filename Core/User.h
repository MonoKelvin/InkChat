#ifndef USER_H
#define USER_H

#include <IChatObject.h>
#include <Utility.h>

#include <QMutex>
#include <QQmlListProperty>

class QJSEngine;
class MyFriend;

class User : public IChatObject {
    Q_OBJECT

    friend class LoginWithQQMail;

    Q_PROPERTY(QString Account READ getAccount WRITE setAccount CONSTANT)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)
    Q_PROPERTY(QQmlListProperty<MyFriend> Friends READ getFriends)

public:
    explicit User(QObject* parent = nullptr);
    ~User() override;

    void fromJson(const QJsonObject& json, bool cache = true) override;
    QJsonObject toJson() override;
    bool loadCache() override;

    inline const QString getAccount(void) const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword(void) const { return mPassword; }

    QQmlListProperty<MyFriend> getFriends(void);

    MyFriend* getFriendById(unsigned int id);

    void addFriend(MyFriend* myFriend);

    inline static User* Instance()
    {
        // 防止多线程多次构造对象
        if (nullptr == mInstance) {
            mMutex.lock();
            if (nullptr == mInstance) {
                mInstance = new User;
            }
            mMutex.unlock();
        }
        return mInstance;
    }

    static QObject* QmlSingletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return User::Instance();
    }

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    // 垃圾回收类
    class _GarbageCollection {
    public:
        _GarbageCollection() = default;
        ~_GarbageCollection()
        {
            // 防止渲染线程和主线程多次析构对象
            if (mInstance) {
                mMutex.lock();
                if (mInstance) {
                    delete mInstance;
                    mInstance = nullptr;
                }
                mMutex.unlock();
            }
        }
    };
    static _GarbageCollection _GC;

private:
    // 账号
    QString mAccount;

    // 密码
    QString mPassword;

    // 我的好友
    QList<MyFriend*> mFriends;

    static QMutex mMutex;
    static User* mInstance;
};

#endif // USER_H

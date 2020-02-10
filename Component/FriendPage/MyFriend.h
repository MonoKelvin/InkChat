#ifndef MYFRIEND_H
#define MYFRIEND_H

#include <IPerson.h>

class User;

class MyFriend : public IPerson
{
    Q_OBJECT
    friend class User;

    Q_PROPERTY(bool isTop READ getIsTop WRITE setIsTop NOTIFY isTopChanged)
    Q_PROPERTY(QString remark READ getRemark WRITE setRemark NOTIFY remarkChanged)
    Q_PROPERTY(QString subgroup READ getSubgroup WRITE setSubgroup NOTIFY subgroupChanged)

public:
    explicit MyFriend(QObject *parent = nullptr);

    void fromJson(const QJsonObject& json) override;
    QJsonObject toJson(void) override;

    inline bool getIsTop() const { return mIsTop; }
    inline void setIsTop(bool isTop)
    {
        mIsTop = isTop;
        emit isTopChanged();
    }

    inline QString getRemark() const { return mRemark; }
    inline void setRemark(const QString &remark)
    {
        mRemark = remark;
        emit remarkChanged();
    }

    inline QString getSubgroup() const { return mSubgroup; }
    inline void setSubgroup(const QString &subgroup)
    {
        mSubgroup = subgroup;
        emit subgroupChanged();
    }

public slots:
    void requestFriendsData(User *user);

Q_SIGNALS:
    void failed(const QString &);

    void isTopChanged();
    void remarkChanged();
    void subgroupChanged();

private:
    bool mIsTop;
    QString mRemark;
    QString mSubgroup;
};

#endif // MYFRIEND_H

#ifndef MYFRIEND_H
#define MYFRIEND_H

#include <IChatObject.h>

class User;

class MyFriend : public IPerson {
    Q_OBJECT
    friend class User;

    Q_PROPERTY(QString remark READ getRemark WRITE setRemark NOTIFY remarkChanged)
    Q_PROPERTY(QString subgroup READ getSubgroup WRITE setSubgroup NOTIFY subgroupChanged)

public:
    explicit MyFriend(QObject *parent = nullptr);

    void fromJson(const QJsonObject& json, bool cache = true) override;
    QJsonObject toJson(void) override;

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

Q_SIGNALS:
    void remarkChanged();
    void subgroupChanged();

private:
    // 备注名
    QString mRemark;

    // 分组名
    QString mSubgroup;
};

#endif // MYFRIEND_H

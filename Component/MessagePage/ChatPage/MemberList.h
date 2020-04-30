#ifndef MEMBERLIST_H
#define MEMBERLIST_H

#include <IChatObject.h>

#include <QAbstractItemDelegate>
#include <QAbstractListModel>
#include <QPointer>

class LanObject;

/**
 * @brief 成员列表模型
 */
class MemberList : public QAbstractListModel {
    Q_OBJECT

public:
    explicit MemberList(LanObject* lan, QObject* parent = nullptr);

    const QModelIndex getIndexByUuid(const QString& uuid);

    LanObject* getLanObject(void) const noexcept;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    /** 局域网对象指针数据 */
    QPointer<LanObject> mLanObject;
};

/**
 * @brief 成员项代理
 */
class MemberItemDelegate : public QAbstractItemDelegate {
public:
    explicit MemberItemDelegate(QObject* parent = nullptr)
        : QAbstractItemDelegate(parent)
    {
    }

    enum { MemberItemRole = Qt::UserRole + 102 };

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // MEMBERLIST_H

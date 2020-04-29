#ifndef MEMBERLIST_H
#define MEMBERLIST_H

#include <IChatObject.h>

#include <QAbstractItemDelegate>
#include <QAbstractListModel>

class MemberList : public QAbstractListModel {
    Q_OBJECT

public:
    explicit MemberList(QObject* parent = nullptr);

    void addMember(const SUserBaseData& member);
    void removeMember(const QString& uuid);

    const QModelIndex getIndexFormUuid(const QString& uuid);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    /** 成员数据 */
    QList<SUserBaseData> mMembers;
};

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

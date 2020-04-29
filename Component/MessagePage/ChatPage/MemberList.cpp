#include "MemberList.h"

#include <AppSettings.h>
#include <AppTheme.h>
#include <Widget/Avatar.h>

#include <QPainter>

MemberList::MemberList(QObject* parent)
    : QAbstractListModel(parent)
{
}

void MemberList::addMember(const SUserBaseData& member)
{
    beginInsertRows(QModelIndex(), mMembers.size(), mMembers.size());
    mMembers.insert(mMembers.size(), member);
    endInsertRows();
}

void MemberList::removeMember(const QString& uuid)
{
    for (int i = 0; i < mMembers.size(); ++i) {
        if (mMembers.at(i).Uuid == uuid) {
            beginRemoveRows(QModelIndex(), i, i);
            mMembers.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

const QModelIndex MemberList::getIndexFormUuid(const QString& uuid)
{
    for (int i = 0; i < mMembers.length(); ++i) {
        if (mMembers.at(i).Uuid == uuid) {
            return index(i);
        }
    }

    return QModelIndex();
}

int MemberList::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return mMembers.size();
}

QVariant MemberList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_UNUSED(role)

    return QVariant::fromValue(mMembers.at(index.row()));
}

QSize MemberItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 70);
}

void MemberItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        // item 数据
        const auto itemData = index.data(MemberItemRole).value<SUserBaseData>();
        const QRect& rect = option.rect;

        painter->save();
        painter->setPen(Qt::NoPen);

        // 鼠标移过
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setBrush(XTheme.TintColor);
            painter->drawRect(rect);
        } else if (option.state.testFlag(QStyle::State_Selected)) { // 选中
            painter->setBrush(XTheme.getPrimarySubColor(1));
            painter->drawRect(rect);
        }

        // 绘制头像
        const QRect avtRect(ESize::Narrow, rect.top() + (rect.height() - XTheme.AvatarSize) / 2, XTheme.AvatarSize, XTheme.AvatarSize);
        Avatar::DrawAvatar(painter, avtRect, AppSettings::AvatarCacheFile(itemData.Uuid), itemData.Name);

        // 绘制名称
        const QRect nameRect(avtRect.right() + ESize::Narrow, rect.top(), rect.width() - avtRect.right() - ESize::Narrow, rect.height());
        painter->setFont(XTheme.SubTitleFont);
        painter->setPen(XTheme.MainTextColor);
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, GetElidedText(itemData.Name, painter->font(), nameRect.width()));

        painter->restore();
    }
}

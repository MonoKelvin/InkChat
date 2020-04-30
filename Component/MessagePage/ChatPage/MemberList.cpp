#include "MemberList.h"

#include <AppSettings.h>
#include <AppTheme.h>
#include <LanObject.h>
#include <Widget/Avatar.h>

#include <QPainter>

MemberList::MemberList(LanObject* lan, QObject* parent)
    : QAbstractListModel(parent)
    , mLanObject(lan)
{
}

const QModelIndex MemberList::getIndexByUuid(const QString& uuid)
{
    const auto& members = mLanObject->getMembers();
    for (int i = 0; i < members.size(); ++i) {
        if (members.at(i).Uuid == uuid) {
            return index(i);
        }
    }

    return QModelIndex();
}

LanObject* MemberList::getLanObject() const noexcept
{
    return mLanObject;
}

int MemberList::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return mLanObject->getMembers().size();
}

QVariant MemberList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_UNUSED(role)

    return QVariant::fromValue(mLanObject->getMembers().at(index.row()));
}

QSize MemberItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 55);
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
        if (option.state.testFlag(QStyle::State_Selected) || option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setBrush(XTheme.TintColor);
            painter->drawRect(rect);
        }
        if (option.state.testFlag(QStyle::State_Selected)) { // 选中
            painter->setBrush(XTheme.PrimayColor1);
            const QRect bar(0, rect.top(), 4, rect.height());
            painter->drawRect(bar);
        }

        // 绘制头像
        const QRect avtRect(ESize::Std, rect.top() + (rect.height() - 36) / 2, 36, 36);
        Avatar::DrawAvatar(painter, avtRect, AppSettings::AvatarCacheFile(itemData.Uuid), itemData.Name);

        // 绘制名称
        const QRect nameRect(avtRect.right() + ESize::Narrow, rect.top(), rect.width() - avtRect.right() - ESize::Narrow, rect.height());
        painter->setFont(XTheme.SubTitleFont);
        painter->setPen(XTheme.MainTextColor);
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, GetElidedText(itemData.Name, painter->font(), nameRect.width()));

        painter->restore();
    }
}

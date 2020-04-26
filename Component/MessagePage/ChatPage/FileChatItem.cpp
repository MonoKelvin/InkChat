#include "FileChatItem.h"

#include <AppTheme.h>
#include <User.h>
#include <Utility.h>

FileChatItem::FileChatItem(QObject* parent)
    : ChatItem(parent)
    , Speed(QStringLiteral("0.0M/s"))
    , Percentage(0)
{
}

void FileChatItem::paintContent(QPainter* painter, const QRect& rect)
{
    constexpr int d = ESize::Std;
    constexpr int sd = ESize::Narrow;

    // 背景
    QRect bg;
    bg.setTopLeft(rect.topLeft());
    bg.setWidth(mContentSize.width());
    bg.setHeight(mContentSize.height() - d);

    painter->save();

    if (getChatItemData().Uuid == User::Instance()->getUuid()) {
        bg.moveRight(rect.right());

        // 时间
        const QRect timeRect(bg.x(), bg.bottom() + ESize::Tiny, qMax(50, mContentSize.width()), ESize::Std);
        painter->setPen(XTheme.SubTextColor);
        painter->setFont(XTheme.SmallFont);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignLeft, getMessageTime());
    } else {
        const QRect timeRect(bg.x(), bg.bottom() + ESize::Tiny, qMax(50, mContentSize.width()), ESize::Std);
        painter->setPen(XTheme.SubTextColor);
        painter->setFont(XTheme.SmallFont);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignRight, getMessageTime());
    }

    // 背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(XTheme.TintColor);
    painter->setRenderHint(QPainter::Antialiasing);
    drawRoundRect(painter, bg, sd, 0, sd, sd);

    // 图标
    const QRect iconRect(bg.left() + d, bg.top() + 10, 50, 50);
    painter->drawPixmap(iconRect, QPixmap(QStringLiteral(":/icons/file_chatitem_64x.png")));

    // 进度条
    QRect proRect(iconRect.right() + d, bg.top() + 50, 145, 4);
    painter->setBrush(XTheme.getPrimarySubColor(1));
    drawRoundRect(painter, proRect, 2, 2, 2, 2);
    proRect.setWidth(int(Percentage * 1.55f));
    painter->setBrush(XTheme.PrimayColor1);
    drawRoundRect(painter, proRect, 2, 2, 2, 2);

    // 文件名
    const QRect nameRect(proRect.left(), bg.top(), 145, 35);
    painter->setPen(XTheme.PrimayColor1);
    painter->setFont(XTheme.StdFont);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawText(nameRect, Qt::AlignBottom | Qt::AlignLeft, getElidedText(getFileNameFromPath(mData.toString()), painter->font(), 145));

    painter->restore();
}

void FileChatItem::updateContentSize(const QStyleOptionViewItem& option)
{
    Q_UNUSED(option)

    mContentSize.setWidth(240);
    mContentSize.setHeight(70 + ESize::Std);
}

bool FileChatItem::openFileDir()
{
    return true;
}

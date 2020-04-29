#include "FileChatItem.h"

#include <AppTheme.h>
#include <User.h>
#include <Utility.h>

FileChatItem::FileChatItem(QObject* parent)
    : ChatItem(parent)
    , Speed(0.0f)
    , Percentage(0)
{
}

void FileChatItem::paintContent(QPainter* painter, const QRect& rect)
{
    const auto& data = getChatItemData();

    // 背景
    QRect bg;
    bg.setTopLeft(rect.topLeft());
    bg.setWidth(mContentSize.width());
    bg.setHeight(mContentSize.height() - ESize::Std);

    painter->save();
    painter->setFont(XTheme.SmallFont);
    painter->setPen(XTheme.SubTextColor);
    painter->setBrush(XTheme.TintColor);

    if (data.Uuid == User::Instance()->getUuid()) {
        bg.moveRight(rect.right());

        // 时间
        const QRect timeRect(bg.x(), bg.bottom() + ESize::Tiny, qMax(50, mContentSize.width()), ESize::Std);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignLeft, getMessageTime());

        // 背景
        painter->setPen(Qt::NoPen);
        painter->setRenderHint(QPainter::Antialiasing);
        DrawRoundRect(painter, bg, 10, 0, 10, 10);
    } else {
        const QRect timeRect(bg.x(), bg.bottom() + ESize::Tiny, qMax(50, mContentSize.width()), ESize::Std);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignRight, getMessageTime());

        painter->setPen(Qt::NoPen);
        painter->setRenderHint(QPainter::Antialiasing);
        DrawRoundRect(painter, bg, 0, 10, 10, 10);
    }

    // 图标
    const QRect iconRect(bg.left() + ESize::Std, bg.top() + 10, 50, 50);
    painter->drawPixmap(iconRect, QPixmap(QStringLiteral(":/icons/file_chatitem_64x.png")));

    // 进度条
    QRect proRect(iconRect.right() + ESize::Std, bg.top() + 56, 145, 4);
    painter->setBrush(XTheme.getPrimarySubColor(1));
    DrawRoundRect(painter, proRect, 2, 2, 2, 2);
    proRect.setWidth(int(Percentage * 1.55f));
    painter->setBrush(XTheme.PrimayColor1);
    DrawRoundRect(painter, proRect, 2, 2, 2, 2);

    // 文件大小
    const QRect sizeRect(proRect.right(), bg.top() + 35, 145, 21);
    painter->setPen(XTheme.SubTextColor);
    painter->drawText(sizeRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("文件大小：") + GetReadableBytes(mFileInfo.size()));

    // 文件名
    const QRect nameRect(proRect.right(), bg.top(), 145, 35);
    painter->setPen(XTheme.PrimayColor1);
    painter->setFont(XTheme.StdFont);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawText(nameRect, Qt::AlignBottom | Qt::AlignLeft, GetElidedText(mFileInfo.fileName(), painter->font(), 145));

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

void FileChatItem::setChatItemData(const SUserBaseData& data) noexcept
{
    mFileInfo.setFile(data.Data.toString());
    return ChatItem::setChatItemData(data);
}

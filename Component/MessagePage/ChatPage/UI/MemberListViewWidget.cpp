#include "MemberListViewWidget.h"

#include <AppTheme.h>
#include <MemberList.h>
#include <MessageManager.h>
#include <User.h>
#include <Utility.h>

#include <QListView>
#include <QPushButton>

MemberListViewWidget::MemberListViewWidget(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    mMemberList = new QListView(this);
    mMemberListModel = new MemberList(this);
    mBtnChat = new QPushButton(this);
    mBtnClose = new QPushButton(this);

    mBtnChat->setObjectName(QStringLiteral("btnChat"));
    mBtnClose->setObjectName(QStringLiteral("btnClose"));

    SHADOW_ICON_SETTER(mBtnChat, "send", XTheme.MainTextColor.lighter(120));
    SHADOW_ICON_SETTER(mBtnClose, "close", XTheme.PrimayColor3.lighter(120));

    mMemberList->setModel(mMemberListModel);
    AttachShadowEffect(mMemberList, 0.0, 4.0, 20.0);

    connect(mMemberList, &QListView::pressed, this, &MemberListViewWidget::updateChatEnabled);
    connect(mBtnClose, &QPushButton::clicked, this, &MemberListViewWidget::deleteLater);
    connect(mBtnChat, &QPushButton::clicked, this, &MemberListViewWidget::requestChatPage);
}

void MemberListViewWidget::updateChatEnabled(const QModelIndex& index)
{
    if (mMemberList->selectionModel()->hasSelection()) {
        auto data = index.data(MemberItemDelegate::MemberItemRole).value<SUserBaseData>();
        if (data.Uuid == User::Instance()->getUuid()) {
            mBtnChat->setEnabled(true);
        }
    }

    mBtnChat->setEnabled(false);
}

void MemberListViewWidget::requestChatPage()
{
    // MessageManager::Instance()->newChatRequest();
}

void MemberListViewWidget::showEvent(QShowEvent*)
{
    resize(320, 500);

    mMemberList->setGeometry(20, 16, 280, 460);
    mBtnClose->move((160 - mBtnClose->width()) / 2, 500 - mBtnClose->height() / 2);
    mBtnChat->move((160 + mBtnChat->width()) / 2, 500 - mBtnChat->height() / 2);

    repaint();
}

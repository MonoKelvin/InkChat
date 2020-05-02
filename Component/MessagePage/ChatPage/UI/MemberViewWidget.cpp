#include "MemberViewWidget.h"

#include <AppTheme.h>
#include <LanObject.h>
#include <MemberList.h>
#include <MessageManager.h>
#include <User.h>
#include <Utility.h>

#include <QBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPushButton>

MemberViewWidget::MemberViewWidget(LanObject* lan, QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint);

    mShadowWidget = new QWidget(this);
    mShadowWidget->setObjectName(QStringLiteral("shadowWidget"));
    AttachShadowEffect(mShadowWidget, 0.0, 8.0, 30.0, QColor("#3654565A"));

    QBoxLayout* vLayout = new QBoxLayout(QBoxLayout::TopToBottom, mShadowWidget);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QBoxLayout* hlHeader = new QBoxLayout(QBoxLayout::LeftToRight);
    hlHeader->setSpacing(10);
    hlHeader->setContentsMargins(10, 0, 10, 0);

    // 刷新按钮
    QPushButton* btnRefresh = new QPushButton(mShadowWidget);
    STD_ICON_SETTER(btnRefresh, "refresh");
    hlHeader->addWidget(btnRefresh);

    // 标题
    QLabel* lbHeader = new QLabel(tr("成员列表"), mShadowWidget);
    lbHeader->setObjectName(QStringLiteral("title"));
    lbHeader->setAlignment(Qt::AlignCenter);
    hlHeader->addWidget(lbHeader);

    // 关闭按钮
    QPushButton* btnClose = new QPushButton(mShadowWidget);
    STD_ICON_SETTER(btnClose, "close");
    hlHeader->addWidget(btnClose);

    vLayout->addLayout(hlHeader);

    // 列表视图
    mMemberList = new QListView(mShadowWidget);
    mMemberListModel = new MemberList(lan, this);
    mMemberList->setModel(mMemberListModel);
    mMemberList->setItemDelegate(new MemberItemDelegate(this));
    mMemberList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mMemberList->setSelectionMode(QAbstractItemView::SingleSelection);
    mMemberList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mMemberList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mMemberList->setResizeMode(QListView::Adjust);
    vLayout->addWidget(mMemberList);

    // 发送按钮
    mBtnChat = new QPushButton(this);
    mBtnChat->setObjectName(QStringLiteral("btnChat"));
    mBtnChat->setEnabled(false);
#ifndef QT_NO_CURSOR
    mBtnChat->setCursor(Qt::ForbiddenCursor);
#endif
    SHADOW_ICON_SETTER(mBtnChat, "send", XTheme.MainTextColor.lighter(150));

#ifndef QT_NO_TOOLTIP
    btnRefresh->setToolTip(tr("刷新"));
    btnClose->setToolTip(tr("关闭面板"));
    mBtnChat->setToolTip(tr("选择上方的成员，点击此按钮进行聊天"));
#endif

    connect(mMemberList, &QListView::pressed, this, &MemberViewWidget::updateChatEnabled);
    connect(mBtnChat, &QPushButton::clicked, this, &MemberViewWidget::requestChatPage);
    connect(btnClose, &QPushButton::clicked, this, &MemberViewWidget::deleteLater);
    connect(btnRefresh, &QPushButton::clicked, this, &MemberViewWidget::refresh);
}

void MemberViewWidget::updateChatEnabled(const QModelIndex& index)
{
    if (mMemberList->selectionModel()->hasSelection()) {
        const auto& data = index.data(MemberItemDelegate::MemberItemRole).value<SUserBaseData>();

        // 选中自己则取消激活聊天按钮
        if (data.Uuid == User::Instance()->getUuid()) {
            mBtnChat->setEnabled(false);
#ifndef QT_NO_CURSOR
            mBtnChat->setCursor(Qt::ForbiddenCursor);
#endif
#ifndef QT_NO_TOOLTIP
            mBtnChat->setToolTip(tr("不可以和自己聊天哦(*^_^*)"));
#endif
        } else {
            mBtnChat->setEnabled(true);
#ifndef QT_NO_CURSOR
            mBtnChat->setCursor(Qt::PointingHandCursor);
#endif
        }
    }
}

void MemberViewWidget::requestChatPage()
{
    const auto& data = mMemberList->currentIndex().data(MemberItemDelegate::MemberItemRole).value<SUserBaseData>();
    const auto& chatObj = User::Instance()->getChatObjectByUuid(data.Uuid, true);

    if (chatObj) {
        chatObj->setNickName(data.Name);
        chatObj->setHostAddress(data.Data.toString());
        emit MessageManager::Instance()->chat(chatObj);
    }

    deleteLater();
}

void MemberViewWidget::refresh()
{
    MessageManager::Instance()->sendMessage(mMemberListModel->getLanObject(), AbstractChatListItem::RequestUserInfo);
}

void MemberViewWidget::resizeEvent(QResizeEvent*)
{
    mBtnChat->move((width() - mBtnChat->width()) / 2, rect().bottom() - mBtnChat->height() - 25);
    mShadowWidget->setGeometry(30, 22, width() - 60, mBtnChat->geometry().center().y() - 22);
}

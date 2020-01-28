#include "FriendInfoCard.h"

#include "../Utility.h"
#include "../Configuation.h"
#include "../User.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsBlurEffect>

FriendInfoCard::FriendInfoCard(QSharedPointer<User> user, QWidget *parent)
    : QWidget(parent)
{
    QBoxLayout *thisLayout;
    QLabel *lbTitle;
    QLabel *lbID;
    QLabel *lbNickName;
    QLabel *lbSignature;
    QGridLayout *gridLayout2;
    QGridLayout *gridLayout;
    QLabel *lbFriendNum;
    QLabel *lbOnlineDaysIcon;
    QLabel *lbFriendNumIcon;
    QLabel *lbOnlineDays;
    QWidget *line;

    thisLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    thisLayout->setSpacing(ESpacing::Narrow);
    thisLayout->setMargin(ESpacing::Std);

    // 标题
    lbTitle = new QLabel(tr("好友信息"), this);
    lbTitle->setObjectName(QStringLiteral("title"));
    lbTitle->setAlignment(Qt::AlignCenter);
    thisLayout->addWidget(lbTitle);

    gridLayout2 = new QGridLayout();
    gridLayout2->setVerticalSpacing(ESpacing::Narrow);

    // id 控件
    lbID = new QLabel("id:" + QString::number(int(user->UID)), this);
    lbID->setTextInteractionFlags(Qt::TextBrowserInteraction);
    lbID->setObjectName(QStringLiteral("idBadge"));
    lbID->setAlignment(Qt::AlignCenter);
    gridLayout2->addWidget(lbID, 0, 0, 1, 1);

    // 头像
    mAvatar = new QLabel(this);
    mAvatar->setPixmap(user->Avatar);
    mAvatar->setObjectName(QStringLiteral("avatar"));
    mAvatar->setScaledContents(true);
    mAvatar->setFixedSize(80, 80);
    mAvatar->raise();   // 放到最前面显示。
    gridLayout2->addWidget(mAvatar, 1, 0, 1, 1);

    thisLayout->addLayout(gridLayout2);

    // 昵称
    lbNickName = new QLabel(user->NickName, this);
    lbNickName->setObjectName(QStringLiteral("name"));
    lbNickName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    lbNickName->setAlignment(Qt::AlignCenter);
    thisLayout->addWidget(lbNickName);

    // 签名
    lbSignature = new QLabel(user->Signature, this);
    lbSignature->setObjectName(QStringLiteral("signature"));
    lbSignature->setTextInteractionFlags(Qt::TextBrowserInteraction);
    lbSignature->setAlignment(Qt::AlignCenter);
    thisLayout->addWidget(lbSignature);

    thisLayout->addStretch(1);

    gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(ESpacing::Narrow);

    // 好友数量icon
    lbFriendNumIcon = new QLabel(this);
    lbFriendNumIcon->setScaledContents(true);
    lbFriendNumIcon->setFixedSize(36, 36);
    lbFriendNumIcon->setObjectName(QStringLiteral("friendNumIcon"));
    gridLayout->addWidget(lbFriendNumIcon, 0, 0, 1, 1);

    // 好友数量（数字）
    lbFriendNum = new QLabel(QStringLiteral("--"), this);
    lbFriendNum->setObjectName(QStringLiteral("friendNum"));
    lbFriendNum->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(lbFriendNum, 1, 0, 1, 1);

    // 在线天数icon
    lbOnlineDaysIcon = new QLabel(this);
    lbOnlineDaysIcon->setScaledContents(true);
    lbOnlineDaysIcon->setFixedSize(36, 36);
    lbOnlineDaysIcon->setObjectName(QStringLiteral("onlineDaysIcon"));
    lbOnlineDaysIcon->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(lbOnlineDaysIcon, 0, 2, 1, 1);

    // 在线天数（数字）
    lbOnlineDays = new QLabel(QStringLiteral("--"), this);
    lbOnlineDays->setObjectName(QStringLiteral("onlineDays"));
    lbOnlineDays->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(lbOnlineDays, 1, 2, 1, 1);

    // 分割线
    line = new QWidget(this);
    line->setObjectName(QStringLiteral("line"));
    gridLayout->addWidget(line, 0, 1, 2, 1);

    thisLayout->addLayout(gridLayout);

    thisLayout->addStretch(1);

    // 编辑备注按钮
    mEditRemark = new QPushButton(tr("编辑备注"), this);
    mEditRemark->setObjectName(QStringLiteral("editRemark"));
    thisLayout->addWidget(mEditRemark);

    // 编辑备注按钮
    mSetGroup = new QPushButton(tr("设置分组"), this);
    mSetGroup->setObjectName(QStringLiteral("setGroup"));
    thisLayout->addWidget(mSetGroup);

    // 编辑备注按钮
    mDeleteFriend = new QPushButton(tr("删除好友"), this);
    mDeleteFriend->setObjectName(QStringLiteral("deleteFriend"));
    thisLayout->addWidget(mDeleteFriend);

    // 头像阴影控件
    QLabel *avatarShadow = new QLabel(this);
    avatarShadow->setPixmap(*mAvatar->pixmap());
    avatarShadow->setObjectName(QStringLiteral("avatarShadow"));
    avatarShadow->setScaledContents(true);
    avatarShadow->setGeometry(mAvatar->geometry());
    avatarShadow->lower(); // 放到最后面显示，防止阴影遮挡头像
    QGraphicsBlurEffect *e = new QGraphicsBlurEffect(avatarShadow);
    e->setBlurRadius(25);
    avatarShadow->setGraphicsEffect(e);
}

void FriendInfoCard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    USE_STYLE_SHEET
//    return QWidget::paintEvent(event);
}

#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include <AppLoginOperation.h>
#include <AppSettings.h>
#include <Configuation.h>
#include <Utility.h>
#include <Widget/PromptWidget.h>

#include <QLabel>
#include <QScreen>
#include <QDir>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // 让界面显示在屏幕中间位置
    QSize lt = QApplication::primaryScreen()->size() - this->size();
    setGeometry(lt.width() / 2, lt.height() / 2, width(), height());
    setWindowTitle(QStringLiteral("InkChat-v%1.%2.%3").arg(CHAT_MAJOR_VERSION).arg(CHAT_MINOR_VERSION).arg(CHAT_REVERSION));
    resize(810, 500);

    ui->lbTitle->setObjectName(QStringLiteral("title"));

    // 登录方式默认为系统方式登录
    mLoginMethod = new AppLoginOperation(this);

    // 初始化相关参数
    mIsSignUp = false;
    mIsExit = false;
    ui->leRecheckPwd->setHidden(true);
    AppSettings::Instance()->CurrentUserId = 0;

    // TODO: 发布时删除
    ui->leNickName->setText("mono");
    ui->lePassword->setText("123123");

    // 左上角的诗句
    // QLabel *verse = new QLabel(this);
    // verse->setObjectName(QStringLiteral("lbVerse"));
    // verse->setText(QStringLiteral("直\n当\n花\n院\n里\n，\n书\n斋\n望\n晓\n开\n。"));
    // verse->setAlignment(Qt::AlignTop);
    // verse->setGeometry(ESize::Std, ESize::Std, 50, height());

    connections();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::loggedIn()
{
    bool loginStatus = false;

    return loginStatus;
}

void LoginDialog::connections()
{
    // 侦听失败消息
    connect(mLoginMethod, &ILoginOperation::failed, [this](const QString& msg) {
        new PromptWidget(msg, this, PromptWidget::Alert);

        ui->btnLogin->setEnabled(true);
        ui->btnSignup->setEnabled(true);
    });

    // 当点击 登录/注册 按钮，进行 验证登录/提交注册
    connect(ui->btnLogin, &QPushButton::clicked, [this] {
        QVariantMap mapping;
        mapping[QStringLiteral("nickName")] = ui->leNickName->text();
        mapping[QStringLiteral("password")] = ui->lePassword->text();

        // 防止多次点击造成重复请求
        ui->btnLogin->setEnabled(false);
        ui->btnSignup->setEnabled(false);

        if (mIsSignUp) {
            if (ui->lePassword->text() != ui->leRecheckPwd->text()) {
                emit mLoginMethod->failed(tr("两次输入的密码不一样！"));
                return;
            }
            mLoginMethod->signupRequest(mapping);
        } else {
            mLoginMethod->loginRequest(mapping);
        }
    });

    // 登录成功
    connect(mLoginMethod, &ILoginOperation::verified, this, &LoginDialog::accept);

    // 注册成功
    connect(mLoginMethod, &ILoginOperation::registered, [this] {
        new PromptWidget(tr("新用户创建成功"), this);

        ui->leRecheckPwd->clear();
        ui->btnLogin->setEnabled(true);
        ui->btnSignup->setEnabled(true);
    });

    // 点击注册按钮实现的登录、注册切换功能
    connect(ui->btnSignup, &QPushButton::clicked, [this] {
        ui->leRecheckPwd->setHidden(mIsSignUp);
        mIsSignUp = !mIsSignUp;

        if (mIsSignUp) {
            ui->lbTitle->setText(tr("新用户"));
            ui->btnLogin->setText(tr("创建用户"));
            ui->btnSignup->setText(tr("返回登录"));
            ui->lePassword->clear();
            ui->leNickName->clear();
            ui->leRecheckPwd->clear();
        } else {
            ui->lbTitle->setText(tr("登录"));
            ui->btnLogin->setText(tr("登录"));
            ui->btnSignup->setText(tr("新建用户"));
        }
    });

    // 当登录页面退出时
    connect(this, &QDialog::rejected, [this] {
        mIsExit = true;
    });
}

#include "SettingsPage.h"
#include "ui_settingspage.h"

#include <AppLoginOperation.h>
#include <AppSettings.h>
#include <AppTheme.h>
#include <User.h>

#include <QDir>
#include <QListView>

SettingsPage::SettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPage)
{
    ui->setupUi(this);

    /** 用户信息页 */
    ui->avatar->setAvatar(AppSettings::UserPrimitiveAvatar());
    ui->lbUuid->setText(QStringLiteral("UUID:") + User::Instance()->getUuid());
    STD_ICON_SETTER(ui->btnPwdEye, "eye_off");
    restore();

    /** 设置页 */
    ui->cbStyleSheet->setView(new QListView(this));
    ui->sbUdpPort->setValue(AppSettings::Value(QStringLiteral("App/lanUdpPort"), LAN_UDP_PORT).toInt());
    ui->sbTcpPort->setValue(AppSettings::Value(QStringLiteral("App/lanTcpPort"), LAN_TCP_PORT).toInt());
    ui->sbMaxChatPage->setValue(AppSettings::Value(QStringLiteral("App/maxChatPageCount"), 3).toInt());
    refreshStyleSheetList(AppSettings::AppThemeDir());

    connect(ui->btnCancel, &QPushButton::clicked, this, &SettingsPage::restore);
    connect(ui->btnSure, &QPushButton::clicked, this, &SettingsPage::onSubmit);
    connect(ui->leName, &QLineEdit::textChanged, this, &SettingsPage::needToChanged);
    connect(ui->teSignature, &QPlainTextEdit::textChanged, this, &SettingsPage::needToChanged);
    connect(ui->lePwd, &QLineEdit::textChanged, this, &SettingsPage::needToChanged);
    connect(ui->cbStyleSheet, &QComboBox::currentTextChanged, this, &SettingsPage::onStyleSheetChanged);
    connect(ui->teSignature, &QPlainTextEdit::textChanged, [this] {
        const auto& str = ui->teSignature->toPlainText();
        if (str.length() > 32) {
            ui->teSignature->setPlainText(str.left(32));
        }
    });
    connect(ui->btnPwdEye, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            ui->lePwd->setEchoMode(QLineEdit::Normal);
            ui->btnPwdEye->setIcon(QIcon::fromTheme(QStringLiteral("eye_open")));
        } else {
            ui->lePwd->setEchoMode(QLineEdit::Password);
            ui->btnPwdEye->setIcon(QIcon::fromTheme(QStringLiteral("eye_off")));
        }
    });
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::refreshStyleSheetList(const QString& path)
{
    ui->cbStyleSheet->clear();

    const auto& fs = QDir(path).entryInfoList(QStringList() << QStringLiteral("*.qss"), QDir::Files);
    for (int i = 0; i < fs.length(); ++i) {
        ui->cbStyleSheet->addItem(fs.at(i).baseName());
    }
}

void SettingsPage::onStyleSheetChanged(const QString& name)
{
    AppSettings::SetValue(QStringLiteral("App/theme"), name);
    QFile f(AppSettings::AppThemeDir() + name);
    if (f.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(f.readAll());
    }
}

void SettingsPage::onSubmit()
{
    const auto& name = ui->leName->text();
    const auto& pwd = ui->lePwd->text();
    const auto& signature = ui->teSignature->toPlainText();

    if (HasIllegalCharInFile(name)) {
        emit failed(tr("数据目录创建失败，用户名可能包含以下非法字符：\\/\"*?<>|"));
    } else if (name.trimmed().length() == 0) {
        emit failed(tr("用户名不可为空"));
    } else if (AppLoginOperation::IsUserExists(name, true)) {
        emit failed(tr("用户名已经存在"));
    } else if (pwd.length() < 6 || pwd.length() > 16) {
        emit failed(tr("请输入6-16位的密码"));
    } else {
        User::Instance()->setNickName(name);
        User::Instance()->setPassword(pwd);
        User::Instance()->setSignature(signature);
        User::Instance()->updateLocalData();
    }

    ui->btnCancel->setVisible(false);
    ui->btnSure->setVisible(false);
    ui->btnSure->updateGeometry();
}

void SettingsPage::needToChanged()
{
    ui->btnCancel->setVisible(true);
    ui->btnSure->setVisible(true);

    ui->btnSure->updateGeometry();
}

void SettingsPage::restore()
{
    ui->leName->setText(User::Instance()->getNickName());
    ui->teSignature->setPlainText(User::Instance()->getSignature());
    ui->lePwd->setText(User::Instance()->getPassword());

    ui->btnCancel->setVisible(false);
    ui->btnSure->setVisible(false);
    ui->btnSure->updateGeometry();
}

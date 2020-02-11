﻿#include <AppSettings.h>
#include <FriendPage.h>
#include <LoginWithQQMail.h>
#include <MessagePage.h>
#include <User.h>

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // 主程序
    QGuiApplication app(argc, argv);

    QFont font = AppSettings::Instance()->value("app/font", "微软雅黑").toString();

#if defined(Q_OS_WIN)
    app.setFont(font);
#endif

    // 注册C++类型到QML
    qmlRegisterType<User>("User", 1, 0, "User");

    LOGINOPERATION_INITIALIZA
    MESSAGEPAGE_INITIALIZA
    FRIENDPAGE_INITIALIZA

    QQmlApplicationEngine engine;
    LoginWithQQMail::InitLoginPage(&engine, QUrl(QStringLiteral("qrc:/LoginPage/LoginPage.qml")));

    return app.exec();
}

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <FriendPage.h>
#include <LoginWithQQMail.h>
#include <MessagePage.h>
#include <User.h>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // 主程序
    QGuiApplication app(argc, argv);

#if defined(Q_OS_WINDOWS)
    app.setFont(QFont(QStringLiteral("微软雅黑")));
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

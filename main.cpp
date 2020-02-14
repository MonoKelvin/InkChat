#include <AppSettings.h>
#include <FriendPage.h>
#include <LoginWithQQMail.h>
#include <MessagePage.h>

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // 主程序
    QGuiApplication app(argc, argv);

    QFont font = AppSettings::Instance()->value("app/font", "微软雅黑").toString();

#if defined(Q_OS_WIN)
    app.setFont(font);
#endif

    {
        auto user = AppSettings::Instance()->value("user/currentUser", 0).toUInt();
        if (user != 0) {
            AppPaths::SetCurrentUserId(user);
        }
    }

    // 注册C++类型到QML
    // qmlRegisterSingletonType<User>("Singleton.User", 1, 0, "User");
    qmlRegisterUncreatableType<IChatObject>("ChatObject", 1, 0, "ChatObject", "Cannot create ChatObject, because it is a interface");

    LOGINOPERATION_INITIALIZA
    MESSAGEPAGE_INITIALIZA
    FRIENDPAGE_INITIALIZA

    QQmlApplicationEngine engine;
    LoginWithQQMail::InitLoginPage(&engine, QUrl(QStringLiteral("qrc:/LoginPage/LoginPage.qml")));

    return app.exec();
}

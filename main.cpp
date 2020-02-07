#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <LoginWithQQMail.h>
#include <MessagePage.h>
#include <Navigation.h>
#include <QDebug>
#include <User.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // 主程序
    QGuiApplication app(argc, argv);

#if defined(Q_OS_WINDOWS)
    app.setFont(QFont(QStringLiteral("微软雅黑")));
#endif

    // 注册C++类型到QML
    qmlRegisterType<User>("User", 1, 0, "User");

    NAVIGATION_INITIALIZA
    MESSAGEPAGE_INITIALIZA

    LoginWithQQMail loginWithQQ;

    return app.exec();
}

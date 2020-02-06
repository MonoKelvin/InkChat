#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <LoginWithQQMail.h>
#include <MessagePage.h>
#include <Navigation.h>
#include <User.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // 主程序
    QGuiApplication app(argc, argv);

#if defined(Q_OS_WINDOWS)
    app.setFont(QFont(QStringLiteral("微软雅黑")));
#endif

    qmlRegisterType<User>("User", 1, 0, "User");
    qmlRegisterType<LoginWithQQMail>("LoginWithQQMail", 1, 0, "LoginWithQQMail");

    NAVIGATION_INITIALIZA
    MESSAGEPAGE_INITIALIZA

    // qml环境
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/LoginPage/LoginPage.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    LoginWithQQMail loginOperation;
    engine.rootContext()->setContextProperty("loginOperation", &loginOperation);

    return app.exec();
}

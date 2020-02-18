#include <AppSettings.h>
#include <LoginWithQQMail.h>

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

#if defined(Q_OS_WIN)
    QFont font = AppSettings::Value(QStringLiteral("app/font"), "微软雅黑").toString();
    app.setFont(font);
#endif

    // 注册C++类型到QML
    qmlRegisterUncreatableType<IChatObject>("ChatObject", 1, 0, "ChatObject", "Cannot create ChatObject, because it is an interface");

    LOGINOPERATION_INITIALIZA

    QQmlApplicationEngine engine;
    LoginWithQQMail::InitLoginPage(&engine, QUrl(QStringLiteral("qrc:/LoginPage/LoginPage.qml")));

    return app.exec();
}

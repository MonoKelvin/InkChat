#include <AppSettings.h>
#include <LoginDelegate.h>

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

#if defined(Q_OS_WIN)
    QFont font = AppSettings::Value(QStringLiteral("App/font"), "微软雅黑").toString();
    app.setFont(font);
#endif

    LOGINDELEGATE_INITIALIZA

    QQmlApplicationEngine engine;
    LoginDelegate::InitLoginPage(&engine, QUrl(QStringLiteral("qrc:/LoginPage/LoginPage.qml")));

    return app.exec();
}

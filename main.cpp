#include "MainWindow.h"

#include <AppSettings.h>
#include <AppTheme.h>
#include <LoginDialog.h>

#include <QApplication>
#include <QFile>

// 全局主题样式
AppTheme XTheme;

int main(int argc, char *argv[])
{
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    {
        // 设置主题
        const auto& theme = QStringLiteral("light");
        AppSettings::SetIconTheme(theme);
        QFile style(AppSettings::AppThemeFile(theme));
        if (style.open(QFile::ReadOnly | QFile::Text)) {
            a.setStyleSheet(style.readAll());
        }
        style.close();

        // 登录页面
        LoginDialog loginPage;
        loginPage.exec();

        // 如果退出登录
        if (loginPage.isExit()) {
            return 0;
        }

    } // 及时释放资源

    // 主页面
    MainWindow w;
    w.show();

    return a.exec();
}

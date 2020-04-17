#include "MainWindow.h"

#include <AppSettings.h>
#include <LoginDialog.h>

#include <QApplication>
#include <QFile>
#include <QPushButton>

int main(int argc, char *argv[])
{
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    AppSettings::SetIconTheme(QStringLiteral("light"));

    {
        // 登录页面
        LoginDialog loginPage;

        //////////////////////////////////////////////////////////
        // 实时调试样式表
        //////////////////////////////////////////////////////////
        QPushButton btn("重新加载样式表", &loginPage);
        btn.show();
        btn.resize(200, 100);
        QObject::connect(&btn, &QPushButton::clicked, [&] {
            QFile f("../InkChat/Resource/Theme/theme_light.qss");
            if (f.open(QFile::ReadOnly)) {
                a.setStyleSheet(f.readAll());
            }
        });
        btn.click();
        //////////////////////////////////////////////////////////

        loginPage.exec();

        // 如果退出登录结束
        if (loginPage.isExit()) {
            return 0;
        }

    } // 及时释放资源

    // 主页面
    MainWindow w;
    w.show();

    return a.exec();
}

#include "MainWindow.h"

#include <AppSettings.h>
#include <LoginDialog.h>

#include <QApplication>
#include <QFile>
#include <QPushButton>

// 实时调试样式表
void debugStyleSheet(QWidget* parent)
{
    QPushButton* btn = new QPushButton("重新加载样式表", parent);
    btn->show();
    btn->resize(120, 40);
    QObject::connect(btn, &QPushButton::clicked, [&] {
        QFile f("../InkChat/Resource/Theme/theme_light.qss");
        if (f.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(f.readAll());
        }
    });
    btn->click();
}

int main(int argc, char *argv[])
{
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    AppSettings::SetIconTheme(QStringLiteral("light"));

    {
        // 登录页面
        LoginDialog loginPage;
        debugStyleSheet(&loginPage);

        loginPage.exec();

        // 如果退出登录结束
        if (loginPage.isExit()) {
            return 0;
        }

    } // 及时释放资源

    // 主页面
    MainWindow w;
    w.show();
    debugStyleSheet(&w);

    return a.exec();
}

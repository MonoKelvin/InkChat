#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QPushButton>

int main(int argc, char *argv[])
{
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);

    MainWindow *w = new MainWindow();
    w->show();
    QPushButton btn("重新加载样式表");
    btn.show();

    QObject::connect(&btn, &QPushButton::clicked, [ = ]
    {
        QFile f("../InkChat/Resource/Theme/theme_light.qss");
        if (f.open(QFile::ReadOnly))
        {
            w->setStyleSheet(f.readAll());
        }
    });

    btn.click();
    a.exec();

    delete w;
    w = nullptr;

    return 0;
}

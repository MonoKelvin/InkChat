#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f("../InkChat/Resource/Theme/theme_light.qss");
    if(f.open(QFile::ReadOnly)){
        a.setStyleSheet(f.readAll());
    } else {
        qDebug() << "Theme file open failed!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}

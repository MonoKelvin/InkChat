#include "Utility.h"

#include <Http/HttpRequest.h>

#include <QDir>
#include <QGraphicsDropShadowEffect>
#include <QLabel>

void attachShadowEffect(QWidget *widget, double xoffset, double yoffset, double radius, const QColor &color)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
    effect->setOffset(xoffset, yoffset);
    effect->setColor(color);
    effect->setBlurRadius(radius);
    widget->setGraphicsEffect(effect);
}

void getElidedText(QLabel *label, int maxWidth, Qt::TextElideMode mode)
{
    label->setFixedWidth(maxWidth);

    const auto fw = label->fontMetrics();
    const auto et = fw.elidedText(label->text(), mode, maxWidth - 3*fw.averageCharWidth());

    label->setText(et);
    label->adjustSize();
}

bool isDirExists(const QString& fullPath, bool makeIfNull, bool recursion)
{
    if (QDir(fullPath).exists()) {
        return true;
    }

    if (makeIfNull) {
        QDir ndir;
        if (recursion) {
            ndir.mkpath(fullPath); // 创建多级目录
            return true;
        }

        return ndir.mkdir(fullPath); // 只创建一级子目录，即必须保证上级目录存在
    }

    return false;
}

bool isFileExists(const QString& fileName, bool makeIfNull, bool recursion)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.isFile()) {
        return true;
    }

    isDirExists(fileInfo.path(), makeIfNull, recursion);

    QFile file(fileName);
    bool ok = file.open(QFile::WriteOnly);
    file.close();

    return ok;
}

void* getPixmapFromNetwork(const QString& url, DealWithPixmapFunc callBack)
{
    HttpRequest* imgRequest = new HttpRequest;
    imgRequest->sendRequest(url);

    //    QObject::connect(imgRequest, &HttpRequest::request, [book](bool success, const QByteArray& data) {
    //        if (success) {
    //            QPixmap pixmap;
    //            if (pixmap.loadFromData(data)) {
    //                book->mImage->setPixmap(pixmap);
    //            }
    //        }
    //    });
}

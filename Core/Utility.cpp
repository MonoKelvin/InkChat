#include "Utility.h"

#include <Http/HttpRequest.h>

#include <QDir>

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

void getPixmapFromNetwork(const QString& url, DealWithPixmapFunc callBack)
{
    HttpRequest* imgRequest = new HttpRequest;
    imgRequest->sendRequest(url);

    QObject::connect(imgRequest, &HttpRequest::request, [callBack](bool success, const QByteArray& data) {
        if (success) {
            QPixmap pixmap;
            if (pixmap.loadFromData(data)) {
                callBack(pixmap, QStringLiteral(""));
                return;
            }
        }

        callBack(QPixmap(), QStringLiteral("IMAGE_LOAD_FAILED"));
    });
}

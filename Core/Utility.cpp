#include "Utility.h"

#include <Http/HttpRequest.h>

#include <QDateTime>
#include <QDir>
#include <QHostInfo>
#include <QNetworkInterface>

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

const QString GetMessageTime(const QDateTime& time)
{
    const auto curTime = QDateTime::currentDateTime();
    const auto days = time.daysTo(curTime);

    if (days == 0) {
        return time.time().toString(QStringLiteral("hh:mm"));
    } else if (days == 1) {
        return QObject::tr("昨天");
    } else if (days == 2) {
        return QObject::tr("前天");
    } else if (days <= 7) {
        return QString::number(days) + QObject::tr("天前");
    } else {
        return time.date().toString(QStringLiteral("yyyy-MM-dd"));
    }
}

quint16 selectAvailablePort(quint16 defaultPort, int maxCount)
{
}

const QString getLocalHostAddress()
{
    foreach (auto address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString();
        }
    }

    return QString();
}

#include "Utility.h"

//#include <HttpRequest.h>

#include <QAbstractButton>
#include <QDateTime>
#include <QDir>
#include <QGraphicsDropShadowEffect>
#include <QHostInfo>
#include <QLabel>
#include <QNetworkInterface>

#include <regex>

void AttachShadowEffect(QWidget *widget, double xoffset, double yoffset, double radius, const QColor &color)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
    effect->setOffset(xoffset, yoffset);
    effect->setColor(color);
    effect->setBlurRadius(radius);
    widget->setGraphicsEffect(effect);
}

void GetElidedText(QLabel *label, int maxWidth, Qt::TextElideMode mode)
{
    label->setFixedWidth(maxWidth);

    const auto fw = label->fontMetrics();
    const auto et = fw.elidedText(label->text(), mode, maxWidth - 3 * fw.averageCharWidth());

    label->setText(et);
    label->adjustSize();
}

bool IsDirExists(const QString& fullPath, bool makeIfNull, bool recursion)
{
    if (QDir(fullPath).exists()) {
        return true;
    }

    if (makeIfNull) {
        QDir ndir;
        if (recursion) {
            return ndir.mkpath(fullPath); // 创建多级目录
        }

        return ndir.mkdir(fullPath); // 只创建一级子目录，即必须保证上级目录存在
    }

    return false;
}

bool IsFileExists(const QString& fileName, bool makeIfNull, bool recursion)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.isFile()) {
        return true;
    }

    IsDirExists(fileInfo.path(), makeIfNull, recursion);

    QFile file(fileName);
    bool ok = file.open(QFile::WriteOnly);
    file.close();

    return ok;
}

void getPixmapFromNetwork(const QString& url, DealWithPixmapFunc callBack)
{
    Q_UNUSED(url)
    Q_UNUSED(callBack)
    /*HttpRequest* imgRequest = new HttpRequest;
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
    });*/
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

quint16 SelectAvailablePort(quint16 defaultPort, int maxCount)
{
    Q_UNUSED(defaultPort)
    Q_UNUSED(maxCount)

    return 0;
}

const QString GetWirelessAddress(QString* mac, QString* netName)
{
    const auto addList = QNetworkInterface::allInterfaces();
    QString hrn;

    for (QNetworkInterface ni : addList) {

        hrn = ni.humanReadableName();

        // 过滤掉不需要的网卡IP地址
        if (-1 != hrn.indexOf(QLatin1String("VMware"))
            || -1 != hrn.indexOf(QLatin1String("Loopback"))
            || -1 != hrn.indexOf(QLatin1String("VirtualBox"))) {
            continue;
        }

        // 挑选激活并使用的IP地址
        if (ni.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)) {
#ifdef Q_OS_LINUX
            // linux环境下 entry.at(1) 是IPv6信息
            const auto& ip = ni.addressEntries().at(0).ip();
#else
            // windows环境下 entry.at(0) 是IPv6信息
            const auto& ip = ni.addressEntries().at(1).ip();
#endif
            if (ip.protocol() == QAbstractSocket::IPv4Protocol
                && -1 != ni.name().indexOf(QLatin1String("wireless"))) {
                if (mac)
                    *mac = ni.hardwareAddress();
                if (netName)
                    *netName = hrn;
                return ip.toString();
            }
        }
    }

    return QString();
}

bool HasIllegalCharInFile(const QString& str)
{
    std::regex reg("[\\s\\\\/:\\*\\?\\\"<>\\|]");
    return std::regex_search(str.toStdString(), reg);
}

bool CopyFile(const QString& source, const QString& newFile, bool isOverWrite)
{
    QFile file(source);
    if (!file.exists()) {
        return false;
    }

    QFileInfo fileInfo(newFile);
    QDir().mkpath(fileInfo.path());

    if (isOverWrite) {
        QFile(newFile).remove(newFile);
    }

    return file.copy(newFile);
}

void switchIconColorForButton(QAbstractButton* btn, const QColor& color)
{
    // TODO
}

void DrawRoundRect(QPainter* painter, const QRect& rect, int lt, int rt, int lb, int rb)
{
    if (rect.width() == 0 || rect.height() == 0) {
        return;
    }

    QPainterPath path;

    path.moveTo(rect.bottomRight() - QPointF(0, rb));
    path.lineTo(rect.topRight() + QPointF(0, rt));
    path.arcTo(QRectF(QPointF(rect.topRight() - QPointF(rt * 2, 0)), QSize(rt * 2, rt * 2)), 0, 90);
    path.lineTo(rect.topLeft() + QPointF(lt, 0));
    path.arcTo(QRectF(QPointF(rect.topLeft()), QSize(lt * 2, lt * 2)), 90, 90);
    path.lineTo(rect.bottomLeft() - QPointF(0, lb));
    path.arcTo(QRectF(QPointF(rect.bottomLeft() - QPointF(0, lb * 2)), QSize(lb * 2, lb * 2)), 180, 90);
    path.lineTo(rect.bottomLeft() + QPointF(lb, 0));
    path.arcTo(QRectF(QPointF(rect.bottomRight() - QPointF(lb * 2, lb * 2)), QSize(lb * 2, lb * 2)), 270, 90);

    painter->fillPath(path, painter->brush());
    painter->drawPath(path);
}

const QString GetHostIP(void)
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    foreach (QHostAddress address, list) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString();
        }
    }

    return QString();
}

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>

class QTimer;
class QNetworkAccessManager;
class QNetworkReply;

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = nullptr);

    enum HttpRequestType
    {
        POST,
        GET
    };

    void sendRequest(const QString &strUrl, HttpRequestType type = GET, const QString &postParams = QString());

Q_SIGNALS:
    // 信号：http请求
    void request(bool bSuccess, const QByteArray &strResult);

private Q_SLOTS:
    // http请求结束
    void requestFinished();

    // 请求超时
    void requestTimeout();

private:
    /** 网络管理类 */
    QNetworkAccessManager *mNetworkManager;

    /** 封装请求返回信息 */
    QNetworkReply *mNetworkReply;

    /** 请求超时计时器 */
    QTimer *mTimer;
};

#endif // HTTPREQUEST_H

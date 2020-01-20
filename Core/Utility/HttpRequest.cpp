#include "HttpRequest.h"

#include <QTimer>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>

#define HTTP_TIMEOUT 6000

HttpRequest::HttpRequest(QObject *parent)
    :QObject (parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
    mNetworkReply = nullptr;
    mTimer = new QTimer(this);
}

void HttpRequest::sendRequest(const QString &strUrl, HttpRequestType type, const QString &postParams)
{
    QNetworkRequest netRequest;

    // 设置请求头和地址信息
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    netRequest.setUrl(QUrl(strUrl));

    // https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    if (strUrl.toLower().startsWith("https")) {
        QSslConfiguration sslConfig;
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfig.setProtocol(QSsl::TlsV1_1);
        netRequest.setSslConfiguration(sslConfig);
    }

    if (type == HttpRequestType::POST) {
        // 发起post请求
        netRequest.setHeader(QNetworkRequest::ContentLengthHeader, postParams.length());
        mNetworkReply = mNetworkManager->post(netRequest, postParams.toUtf8());
    } else {
        // 发起get请求
        mNetworkReply = mNetworkManager->get(netRequest);
    }

    // 开始计时，每HTTP_TIMEOUT个时间戳响应一次信号事件，响应一次后就表示请求超时
    mTimer->start(HTTP_TIMEOUT);

    // 请求完成信号
    connect(mNetworkReply, &QNetworkReply::finished, this, &HttpRequest::requestFinished);

    // 超时信号
    connect(mTimer, &QTimer::timeout, this, &HttpRequest::requestTimeout);
}

void HttpRequest::requestFinished()
{
    mTimer->stop(); //关闭定时器

    //QTextCodec *pCodec = QTextCodec::codecForName("UTF-8");
    //QString strResult = pCodec->toUnicode(mNetworkReply->readAll());
    QByteArray strResult = mNetworkReply->readAll();

    // http返回状态码
    int nHttpCode = mNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    // 返回状态码为200表示成功
    if (nHttpCode == 200) {
        emit request(true, strResult); //请求成功
    } else {
        emit request(false, strResult); //请求失败
    }

    mNetworkReply->deleteLater();
    this->deleteLater(); //释放内存
}

//请求超时
void HttpRequest::requestTimeout()
{
    emit request(false, "timeout"); //请求失败
    mNetworkReply->deleteLater();
    this->deleteLater(); //释放内存
}


#include "networkaccessmanager.h"
#include <QSslConfiguration>
#include <QRegExp>
#include <QDebug>

NetWorkAccessManager::NetWorkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{
    cookieJar = new CookieJar(this);
    setCookieJar(cookieJar);
}

NetWorkAccessManager::~NetWorkAccessManager()
{

}

QNetworkReply * NetWorkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QString url = request.url().toString();
    qDebug() << url;
//    if(url.contains(QRegExp(".*(png|gif|jpg).*")))
//    {
//        QNetworkRequest req;
//        req.setUrl(QUrl(""));
//        return QNetworkAccessManager::createRequest(op, req, outgoingData);
//    }
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setProtocol(QSsl::AnyProtocol);
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

CookieJar * NetWorkAccessManager::getCookieJar()
{
    return cookieJar;
}

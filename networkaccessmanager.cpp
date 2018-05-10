#include "networkaccessmanager.h"
#include <QSslConfiguration>
#include <QRegExp>
#include <QDebug>

NetWorkAccessManager::NetWorkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{
    extractMap = new QMap<QString, QByteArray>();
    cookieJar = new CookieJar(this);
    setCookieJar(cookieJar);
}

NetWorkAccessManager::~NetWorkAccessManager()
{

}

QNetworkReply * NetWorkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QString path = request.url().path();
    qDebug() << path;
    if(!this->interceptor.isNull() && !this->interceptor.isEmpty() && path.contains(QRegExp(this->interceptor)))
    {
        QNetworkRequest req;
        req.setUrl(QUrl(""));
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setProtocol(QSsl::AnyProtocol);
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    if(!this->extractor.isNull() && !this->extractor.isEmpty() && path.contains(QRegExp(this->extractor)))
    {
        connect(reply, &QNetworkReply::readyRead, [=](){
            QByteArray array = reply->peek(10240);
            if(extractMap->contains(path))
            {
                extractMap->remove(path);
            }
            extractMap->insert(path, array.toBase64());
        });
    }
    return reply;
}

CookieJar * NetWorkAccessManager::getCookieJar()
{
    return cookieJar;
}

void NetWorkAccessManager::setInterceptor(QString &interceptor)
{
    this->interceptor = interceptor;
}

QMap<QString, QByteArray> * NetWorkAccessManager::getExtractMap()
{
    return this->extractMap;
}

void NetWorkAccessManager::setExtractor(QString &extractor)
{
    this->extractor = extractor;
}

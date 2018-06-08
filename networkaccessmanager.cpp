#include "networkaccessmanager.h"
#include <QRegExp>

NetWorkAccessManager::NetWorkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{
    extractMap = new QMultiMap<QString, QByteArray>();
    cookieJar = new CookieJar;
    setCookieJar(cookieJar);
}

NetWorkAccessManager::~NetWorkAccessManager()
{
    delete cookieJar;
}

QNetworkReply * NetWorkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QString url = request.url().toString();
    if(!this->interceptor.isNull() && !this->interceptor.isEmpty() && url.contains(QRegExp(this->interceptor)))
    {
        QNetworkRequest req;
        req.setUrl(QUrl(""));
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    QString path = request.url().path();
    if(!path.contains(QRegExp(".*(gif|jpg|png|css|js).*")))
    {
        qDebug() << path << " will be save ";
        if(reply->bytesAvailable() > 0) {
            extractMap->insertMulti(path, reply->peek(reply->bytesAvailable()).toBase64());
        } else {
            QByteArray * data = new QByteArray;
            connect(reply, &QNetworkReply::readyRead, [reply, data, path](){
                qint64 size = reply->bytesAvailable();
                QByteArray array = reply->peek(size);
                data->append(array);
            });
            connect(reply, &QNetworkReply::finished, [=](){
                extractMap->insertMulti(path, (*data).toBase64());
                delete data;
            });
        }
    }

    return reply;
}

CookieJar * NetWorkAccessManager::getCookieJar()
{
    return cookieJar;
}

QMultiMap<QString, QByteArray> * NetWorkAccessManager::getExtractMap()
{
    return this->extractMap;
}

void NetWorkAccessManager::setInterceptor(QString &interceptor)
{
    this->interceptor = interceptor;
}

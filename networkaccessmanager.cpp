#include "networkaccessmanager.h"
#include <QRegExp>

NetWorkAccessManager::NetWorkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{
    extractMap = new QMap<QString, QByteArray>();
    cookieJar = new CookieJar;
    setCookieJar(cookieJar);
}

NetWorkAccessManager::~NetWorkAccessManager()
{
    delete cookieJar;
}

void NetWorkAccessManager::clearAllCookie()
{
    cookieJar->deleteLater();
    cookieJar = new CookieJar;
    setCookieJar(cookieJar);
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

    if(!request.url().path().contains(QRegExp(".*(gif|jpg|png|css|js).*")))
    {
        if(outgoingData != 0) {
            if(outgoingData->isReadable()) {
               QByteArray formData = outgoingData->peek(4096);
               url.append("?").append(formData);
            }
        }
        QCryptographicHash md5(QCryptographicHash::Md5);
        md5.addData(url.toUtf8());
        QByteArray key = md5.result().toHex();

        QMap<QString, QByteArray>::iterator it = extractMap->find(key);
        if(it != extractMap->end())
        {
            extractMap->erase(it);
        }

        QByteArray * data = new QByteArray;
        connect(reply, &QNetworkReply::readyRead, [reply, data](){
            qint64 size = reply->bytesAvailable();
            QByteArray array = reply->peek(size);
            data->append(array);
        });
        connect(reply, &QNetworkReply::finished, [=](){
            qDebug() << url << key << "finished";
            extractMap->insert(key, (*data).toBase64());
            delete data;
        });
    }

    return reply;
}

CookieJar * NetWorkAccessManager::getCookieJar()
{
    return cookieJar;
}

QMap<QString, QByteArray> * NetWorkAccessManager::getExtractMap()
{
    return this->extractMap;
}

void NetWorkAccessManager::setInterceptor(QString &interceptor)
{
    this->interceptor = interceptor;
}

#include "networkaccessmanager.h"
#include <QRegExp>

NetWorkAccessManager::NetWorkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{
    extractMap = new QMap<QString, QByteArray>();
    extractStatusMap = new QMap<QString, int>();
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
    QString path = request.url().path();
    if(!this->interceptor.isNull() && !this->interceptor.isEmpty() && path.contains(QRegExp(this->interceptor)))
    {
        QNetworkRequest req;
        req.setUrl(QUrl(""));
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    if(!this->extractor.isNull() && !this->extractor.isEmpty() && path.contains(QRegExp(this->extractor)))
    {
        QMap<QString, QByteArray>::iterator extractMapIt = extractMap->find(path);
        if(extractMapIt != extractMap->end())
        {
            extractMap->erase(extractMapIt);
        }

        connect(reply, &QNetworkReply::readyRead, [=](){
            qint64 size = reply->bytesAvailable();
            QByteArray array = reply->peek(size);
            if(extractMap->contains(path))
            {
                QMap<QString, QByteArray>::iterator it = extractMap->find(path);
                it.value().append(array);
            }
            else
            {
                extractMap->insert(path, array);
            }

        });
        connect(reply, &QNetworkReply::finished, [=](){
            QMap<QString, int>::iterator extractStatusMapIt = extractStatusMap->find(path);
            if(extractStatusMapIt != extractStatusMap->end())
            {
                extractStatusMapIt.value() = extractStatusMapIt.value() + 1;
            }
            else
            {
                extractStatusMap->insert(path, 1);
            }

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

QMap<QString, int> * NetWorkAccessManager::getExtractStatusMap()
{
    return this->extractStatusMap;
}

void NetWorkAccessManager::setExtractor(QString &extractor)
{
    this->extractor = extractor;
}

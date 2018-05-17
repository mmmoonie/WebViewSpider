#include "networkaccessmanager.h"
#include <QSslConfiguration>
#include <QRegExp>

NetWorkAccessManager::NetWorkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{
    extractMap = new QMap<QString, QByteArray>();
    extractStatusMap = new QMap<QString, bool>();
    cookieJar = new CookieJar(this);
    setCookieJar(cookieJar);
}

NetWorkAccessManager::~NetWorkAccessManager()
{

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
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setProtocol(QSsl::AnyProtocol);
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    if(!this->extractor.isNull() && !this->extractor.isEmpty() && path.contains(QRegExp(this->extractor)))
    {
        QMap<QString, QByteArray>::iterator extractMapIt = extractMap->find(path);
        if(extractMapIt != extractMap->end())
        {
            extractMap->erase(extractMapIt);
        }
        QMap<QString, bool>::iterator extractStatusMapIt = extractStatusMap->find(path);
        if(extractStatusMapIt != extractStatusMap->end())
        {
            extractStatusMap->erase(extractStatusMapIt);
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
            extractStatusMap->insert(path, true);
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

QMap<QString, bool> * NetWorkAccessManager::getExtractStatusMap()
{
    return this->extractStatusMap;
}

void NetWorkAccessManager::setExtractor(QString &extractor)
{
    this->extractor = extractor;
}

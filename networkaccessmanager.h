#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMultiMap>
#include <QNetworkDiskCache>
#include "cookiejar.h"

class NetWorkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetWorkAccessManager(QObject *parent = 0);
    ~NetWorkAccessManager();
    CookieJar * getCookieJar();
    void setInterceptor(QString &interceptor);
    QMultiMap<QString, QByteArray> * getExtractMap();
    QList<QString> * getExtractors();

signals:

public slots:

private:
    CookieJar * cookieJar;
    QString interceptor;
    QMultiMap<QString, QByteArray> * extractMap;
    QList<QString> * extractors;
    QNetworkDiskCache* diskCache;

protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKACCESSMANAGER_H

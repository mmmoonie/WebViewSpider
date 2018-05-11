#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMap>
#include "cookiejar.h"

class NetWorkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetWorkAccessManager(QObject *parent = 0);
    ~NetWorkAccessManager();
    CookieJar * getCookieJar();
    void setInterceptor(QString &interceptor);
    void setExtractor(QString &extractor);
    QMap<QString, QByteArray> * getExtractMap();
    QMap<QString, bool> * getExtractStatusMap();

signals:

public slots:

private:
    CookieJar * cookieJar;
    QString interceptor;
    QString extractor;
    QMap<QString, QByteArray> * extractMap;
    QMap<QString, bool> * extractStatusMap;

protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKACCESSMANAGER_H

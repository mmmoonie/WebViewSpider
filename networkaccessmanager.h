#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "cookiejar.h"

class NetWorkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetWorkAccessManager(QObject *parent = 0);
    ~NetWorkAccessManager();
    CookieJar * getCookieJar();

signals:

public slots:

private:
    CookieJar * cookieJar;

protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKACCESSMANAGER_H

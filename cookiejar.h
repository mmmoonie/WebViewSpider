#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QObject>
#include <QNetworkCookieJar>
#include <QList>
#include <QNetworkCookie>

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit CookieJar(QObject *parent = 0);
    ~CookieJar();
    QList<QNetworkCookie> getAllCookies();

signals:

public slots:

};

#endif // COOKIEJAR_H

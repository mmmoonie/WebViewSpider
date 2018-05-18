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
    QList<QNetworkCookie> cookiesForUrl(const QUrl &url);
    bool deleteCookie(const QNetworkCookie &cookie);
    bool insertCookie(const QNetworkCookie &cookie);
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);
    bool updateCookie(const QNetworkCookie &cookie);

signals:

public slots:

};

#endif // COOKIEJAR_H

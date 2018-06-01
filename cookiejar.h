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
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);
    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
    bool insertCookie(const QNetworkCookie &cookie);
    bool deleteCookie(const QNetworkCookie &cookie);
    bool updateCookie(const QNetworkCookie &cookie);
    void deleteCookiesFromUrl(const QUrl &url);

signals:

public slots:

};

#endif // COOKIEJAR_H

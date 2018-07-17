#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QObject>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit CookieJar(QObject *parent = 0);
    ~CookieJar();
    QList<QNetworkCookie> getAllCookies() const;
    bool insertOneCookie(const QNetworkCookie &cookie);
    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;

signals:

public slots:

};

#endif // COOKIEJAR_H

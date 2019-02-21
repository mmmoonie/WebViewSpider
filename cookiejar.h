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

public:
    QList<QNetworkCookie> cookies(const QUrl &url) const;
    bool addCookie(const QNetworkCookie &cookie, const QUrl &url);
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) Q_DECL_OVERRIDE;

public slots:
    void clearCookies();
    bool deleteCookie(const QNetworkCookie &cookie) Q_DECL_OVERRIDE;

};

#endif // COOKIEJAR_H

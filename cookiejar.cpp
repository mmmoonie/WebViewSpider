#include "cookiejar.h"
#include <QDebug>

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

CookieJar::~CookieJar()
{

}

QList<QNetworkCookie> CookieJar::getAllCookies() const
{
    return allCookies();
}

QList<QNetworkCookie> CookieJar::cookies(const QUrl &url) const {
    if(url.isEmpty() || !url.isValid()) {
        return allCookies();
    } else {
        return cookiesForUrl(url);
    }
}

bool CookieJar::deleteCookie(const QNetworkCookie &cookie) {
    bool deleted = false;
    QList<QNetworkCookie> cookis = allCookies();
    for(int i = 0; i < cookis.size(); i ++) {
        QNetworkCookie networkCookie = cookis.at(i);
        if(networkCookie.name() == cookie.name() &&
                networkCookie.domain() == cookie.domain() &&
                networkCookie.path() == cookie.path()) {
            cookis.removeAt(i);
            deleted = true;
        }
    }
    if(deleted) {
        setAllCookies(cookis);
    }
}

bool CookieJar::addCookie(const QNetworkCookie &cookie, const QUrl &url) {
    QUrl convertedUrl = !url.isEmpty() ?
                url :
                QUrl(QString((cookie.isSecure() ? "https://" : "http://")
                             + QString((cookie.domain().startsWith('.') ? "www" : ""))
                             + cookie.domain()
                             + (cookie.path().isEmpty() ? "/" : cookie.path())));
    return setCookiesFromUrl(QList<QNetworkCookie>() << cookie, convertedUrl);
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

void CookieJar::clearCookies() {
    setAllCookies(QList<QNetworkCookie>());
}

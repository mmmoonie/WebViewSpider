#include "cookiejar.h"
#include <QDebug>

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

CookieJar::~CookieJar()
{

}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl &url) const
{
    QString host = url.host();
    QList<QNetworkCookie> allCookie = allCookies();
    QList<QNetworkCookie> cookies;
    foreach(QNetworkCookie cookie, allCookie) {
        if(cookie.domain().contains(host) || host.contains(cookie.domain())) {
            cookies += cookie;
        }
    }
    return cookies;
}

bool CookieJar::insertCookie(const QNetworkCookie &cookie)
{
    QList<QNetworkCookie> cookieList = allCookies();
    bool exist = false;
    foreach(QNetworkCookie networkCookie, cookieList) {
        if(networkCookie.domain() == cookie.domain() && networkCookie.name() == cookie.name()) {
            exist = true;
            networkCookie = cookie;
            break;
        }
    }
    if(!exist) {
       cookieList << cookie;
    }
    setAllCookies(cookieList);
    return true;
}

bool CookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    QList<QNetworkCookie> cookieList = allCookies();
    foreach(QNetworkCookie networkCookie, cookieList) {
        if(networkCookie.domain() == cookie.domain() && networkCookie.name() == cookie.name()) {
            cookieList.removeOne(networkCookie);
        }
    }
    setAllCookies(cookieList);
    return true;
}

void CookieJar::deleteCookiesFromUrl(const QUrl &url)
{
    QList<QNetworkCookie> cookies = cookiesForUrl(url);
    foreach(QNetworkCookie cookie, cookies) {
        deleteCookie(cookie);
    }
}

bool CookieJar::updateCookie(const QNetworkCookie &cookie)
{
    return insertCookie(cookie);
}

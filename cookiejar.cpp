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

bool CookieJar::insertOneCookie(const QNetworkCookie &cookie)
{
    QList<QNetworkCookie> cookieList = allCookies();
    bool exist = false;
    foreach(QNetworkCookie networkCookie, cookieList) {
        if(networkCookie.name() == cookie.name()) {
            exist = true;
            networkCookie.setValue(cookie.value());
            updateCookie(networkCookie);
            break;
        }
    }
    if(!exist) {
        cookieList << cookie;
        setAllCookies(cookieList);
    }
    return true;
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl &url) const
{
    return allCookies();
}

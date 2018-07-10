#include "cookiejar.h"
#include <QDebug>

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

CookieJar::~CookieJar()
{

}

QList<QNetworkCookie> CookieJar::getCookies(const QUrl &url) const
{
    QList<QNetworkCookie> allCookie = allCookies();
    return allCookie;
}

bool CookieJar::insertOneCookie(const QNetworkCookie &cookie)
{
    QList<QNetworkCookie> cookieList = allCookies();
    bool exist = false;
    foreach(QNetworkCookie networkCookie, cookieList) {
        if((cookie.domain().contains(networkCookie.domain()) || networkCookie.domain().contains(cookie.domain()))
                && networkCookie.name() == cookie.name()) {
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

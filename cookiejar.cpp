#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

CookieJar::~CookieJar()
{

}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl &url)
{
    return QNetworkCookieJar::cookiesForUrl(url);
}

bool CookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::deleteCookie(cookie);
}

bool CookieJar::insertCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::insertCookie(cookie);
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

bool CookieJar::updateCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::updateCookie(cookie);
}

QList<QNetworkCookie> CookieJar::getAllCookies()
{
    return allCookies();
}

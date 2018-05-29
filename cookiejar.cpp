#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

CookieJar::~CookieJar()
{

}

bool CookieJar::updateCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::updateCookie(cookie);
}

QList<QNetworkCookie> CookieJar::getAllCookies()
{
    return allCookies();
}

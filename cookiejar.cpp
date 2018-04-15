#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

CookieJar::~CookieJar()
{

}

QList<QNetworkCookie> CookieJar::getAllCookies()
{
    return allCookies();
}

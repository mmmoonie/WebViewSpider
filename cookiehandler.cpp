#include "cookiehandler.h"
#include <QJsonArray>

CookieHandler::CookieHandler(WebView * webView)
{
    this->webView = webView;
}

CookieHandler::~CookieHandler()
{

}

QJsonObject CookieHandler::getAllCookies()
{
    CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
    QJsonArray cookieArray;
    QList<QNetworkCookie> cookieList = cookieJar->cookiesForUrl(webView->url());
    for(int i = 0; i < cookieList.size(); i ++)
    {
        QNetworkCookie cookie = cookieList.at(i);
        QJsonObject cookieJson;
        cookieJson.insert("name", QString(cookie.name()));
        cookieJson.insert("value", QString(cookie.value()));
        cookieJson.insert("domain", QJsonValue(cookie.domain()));
        cookieJson.insert("path", QJsonValue(cookie.path()));
        cookieJson.insert("expirationDate", QJsonValue(cookie.expirationDate().toMSecsSinceEpoch()));
        cookieJson.insert("httpOnly", QJsonValue(cookie.isHttpOnly()));
        cookieJson.insert("secure", QJsonValue(cookie.isSecure()));
        cookieArray.append(cookieJson);
    }
    QJsonObject json;
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue(cookieArray));
    return json;
}

QJsonObject CookieHandler::setCookies(QJsonArray &cookieArray)
{
    CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
    for(int i = 0; i < cookieArray.size(); i ++)
    {
        QJsonObject cookieObj = cookieArray.at(i).toObject();
        QString name = cookieObj.value("name").toString();
        QString value = cookieObj.value("value").toString();
        QString domain = cookieObj.value("domain").toString();
        QString path = cookieObj.value("path").toString();
        qint64 expirationDate = cookieObj.value("expirationDate").toInt();
        bool httpOnly = cookieObj.value("httpOnly").toBool();
        bool secure = cookieObj.value("secure").toBool();
        QNetworkCookie cookie;
        cookie.setName(name.toUtf8());
        cookie.setValue(value.toUtf8());
        cookie.setDomain(domain);
        cookie.setPath(path);
        cookie.setExpirationDate(QDateTime::fromMSecsSinceEpoch(expirationDate));
        cookie.setHttpOnly(httpOnly);
        cookie.setSecure(secure);
        cookieJar->insertCookie(cookie);
    }
    QJsonObject json;
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue::Null);
    return json;
}

QJsonObject CookieHandler::deleteCookiesFromUrl(QString &url)
{
    QJsonObject json;
    CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
    if(url.isEmpty()) {
        cookieJar->deleteCookiesFromUrl(webView->url());
    } else {
        cookieJar->deleteCookiesFromUrl(QUrl::fromUserInput(url));
    }
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue::Null);
    return json;
}


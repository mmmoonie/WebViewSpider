#ifndef COOKIEHANDLER_H
#define COOKIEHANDLER_H

#include "webview.h"
#include <QJsonObject>

class CookieHandler
{
public:
    CookieHandler(WebView * webView);
    ~CookieHandler();
    QJsonObject getAllCookies();
    QJsonObject setCookies(QJsonArray &cookieArray);
    QJsonObject deleteCookiesFromUrl(QString &url);

private:
    WebView * webView = 0;
};

#endif // COOKIEHANDLER_H

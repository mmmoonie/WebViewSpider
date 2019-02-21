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
    QJsonObject insertCookies(QJsonArray& cookieArray, QUrl &url);
    QJsonObject addCookies(QJsonArray &cookieArray);
    QJsonObject deleteAllCookies();

private:
    WebView * webView = 0;
};

#endif // COOKIEHANDLER_H

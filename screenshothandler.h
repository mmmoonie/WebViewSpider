#ifndef SCREENSHOTHANDLER_H
#define SCREENSHOTHANDLER_H

#include "webview.h"
#include <QJsonObject>

class ScreenshotHandler
{
public:
    ScreenshotHandler(WebView * webView = 0);
    ~ScreenshotHandler();
    QJsonObject handle(const QString &selector);

private:
    WebView * webView;
};

#endif // SCREENSHOTHANDLER_H

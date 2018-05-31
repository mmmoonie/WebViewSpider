#ifndef LOADHANDLER_H
#define LOADHANDLER_H

#include <QJsonObject>
#include "webview.h"

class LoadHandler
{
public:
    LoadHandler(WebView * webView = 0);
    QJsonObject handle(QJsonObject &dataJson);
    ~LoadHandler();

private:
    WebView * webView;
};

#endif // LOADHANDLER_H

#ifndef EXECHANDLER_H
#define EXECHANDLER_H

#include "webview.h"
#include <QJsonObject>

class ExecHandler
{
public:
    ExecHandler(WebView * webView = 0);
    ~ExecHandler();
    QJsonObject handle(QString &javaScript);

private:
    WebView * webView;
};

#endif // EXECHANDLER_H

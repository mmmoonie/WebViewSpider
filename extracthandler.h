#ifndef EXTRACTHANDLER_H
#define EXTRACTHANDLER_H

#include "webview.h"

class ExtractHandler
{
public:
    ExtractHandler(WebView * webView = 0);
    QByteArray handle(QString key);
    ~ExtractHandler();

private:
    WebView * webView;
};

#endif // EXTRACTHANDLER_H

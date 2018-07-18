#ifndef EXTRACTHANDLER_H
#define EXTRACTHANDLER_H

#include "webview.h"
#include <QJsonObject>
#include <QJsonArray>

class ExtractHandler
{
public:
    ExtractHandler(WebView * webView = 0);
    QJsonObject handle(QString key, int count);
    ~ExtractHandler();
    bool addExtractor(QString &extractor);
    bool delExtractor(QString &extractor);

private:
    WebView * webView;
};

#endif // EXTRACTHANDLER_H

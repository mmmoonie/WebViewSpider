#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include "webpage.h"

class WebView : public QWebView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);
    ~WebView();
    WebPage * getWebPage();

signals:

public slots:

private:
    WebPage * webPage;
};

#endif // WEBVIEW_H

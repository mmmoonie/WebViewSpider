#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QWebInspector>
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
    QWebInspector *inspector = 0;
};

#endif // WEBVIEW_H

#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
    webPage = new WebPage(this);
    setPage(webPage);
    inspector = new QWebInspector;
    inspector->setPage(webPage);
    inspector->show();
}

WebView::~WebView()
{

}

WebPage * WebView::getWebPage()
{
    return webPage;
}

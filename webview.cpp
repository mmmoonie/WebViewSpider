#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
    webPage = new WebPage(this);
    setPage(webPage);
}

WebView::~WebView()
{

}

WebPage * WebView::getWebPage()
{
    return webPage;
}

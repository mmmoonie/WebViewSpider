#include "webview.h"
#include <QApplication>
#include <QDesktopWidget>

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

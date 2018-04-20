#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
    webPage = new WebPage(this);
    setPage(webPage);
#ifdef QT_DEBUG
    inspector = new QWebInspector;
    inspector->setPage(webPage);
    inspector->show();
#endif
}

WebView::~WebView()
{

}

WebPage * WebView::getWebPage()
{
    return webPage;
}

#include "webview.h"
#include <QApplication>
#include <QDesktopWidget>

WebView::WebView(QWidget *parent) : QWebView(parent)
{
    webPage = new WebPage(this);
    setPage(webPage);
    inspector = new QWebInspector;
    inspector->setPage(webPage);
    inspector->show();
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->availableGeometry();
    int width = screenRect.width();
    int height = screenRect.height();
    inspector->resize(width/2, height);
    inspector->move(width/2 + 2, 0);
}

WebView::~WebView()
{

}

WebPage * WebView::getWebPage()
{
    return webPage;
}

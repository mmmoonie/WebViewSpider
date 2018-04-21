#include "webview.h"
#include <QApplication>
#include <QDesktopWidget>

WebView::WebView(QWidget *parent) : QWebView(parent)
{
    webPage = new WebPage(this);
    setPage(webPage);
#ifdef QT_DEBUG
    inspector = new QWebInspector;
    inspector->setPage(webPage);
    inspector->show();
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->availableGeometry();
    int width = screenRect.width();
    int height = screenRect.height();
    inspector->resize(width/2, height);
    inspector->move(width/2 + 2, 0);
#endif
}

WebView::~WebView()
{

}

WebPage * WebView::getWebPage()
{
    return webPage;
}

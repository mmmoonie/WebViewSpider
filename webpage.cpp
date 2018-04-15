#include "webpage.h"
#include <QDebug>
#include <QDir>

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
    networkAccessManager = new NetWorkAccessManager(this);
    setNetworkAccessManager(networkAccessManager);
    setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

WebPage::~WebPage()
{

}

void WebPage::javaScriptAlert(QWebFrame *frame, const QString &msg)
{

    qDebug() << msg;
}

NetWorkAccessManager * WebPage::getNetworkAccessManager()
{
    return networkAccessManager;
}

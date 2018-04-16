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

QString WebPage::userAgentForUrl(const QUrl &url) const
{
    return "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36";
}

NetWorkAccessManager * WebPage::getNetworkAccessManager()
{
    return networkAccessManager;
}

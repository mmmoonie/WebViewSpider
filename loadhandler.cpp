#include "loadhandler.h"
#include <QNetworkProxy>

LoadHandler::LoadHandler(WebView * webView)
{
    this->webView = webView;
}

LoadHandler::~LoadHandler()
{

}

QJsonObject LoadHandler::handle(QJsonObject &dataJson)
{
    if(dataJson.contains("interceptor"))
    {
        QString interceptor = dataJson.value("interceptor").toString();
        webView->getWebPage()->getNetworkAccessManager()->setInterceptor(interceptor);
    }
    if(dataJson.contains("proxy"))
    {
        QJsonObject proxyJson = dataJson.value("proxy").toObject();
        QString type = proxyJson.value("type").toString();
        QString ip = proxyJson.value("ip").toString();
        int port = proxyJson.value("port").toInt();
        QNetworkProxy proxy;
        if(type == "socks5")
        {
            proxy.setType(QNetworkProxy::Socks5Proxy);
        }
        else
        {
            proxy.setType(QNetworkProxy::HttpProxy);
        }
        proxy.setHostName(ip);
        proxy.setPort(port);
        QNetworkProxy::setApplicationProxy(proxy);
    }
    else
    {
        QNetworkProxyFactory::setUseSystemConfiguration(true);
    }
    QUrl url = QUrl::fromUserInput(dataJson.value("url").toString("about:blank").toLocal8Bit());
    this->webView->getWebPage()->getNetworkAccessManager()->getExtractMap()->clear();
    webView->setUrl(url);
    QJsonObject json;
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", webView->url().toString());
    return json;
}

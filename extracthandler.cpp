#include "extracthandler.h"

ExtractHandler::ExtractHandler(WebView * webView)
{
    this->webView = webView;
}

ExtractHandler::~ExtractHandler()
{

}

QByteArray ExtractHandler::handle(QString key)
{
    NetWorkAccessManager * accessManager = webView->getWebPage()->getNetworkAccessManager();
    QMap<QString, QByteArray> * dataMap = accessManager->getExtractMap();
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(key.toUtf8());
    QMap<QString, QByteArray>::iterator it = dataMap->find(md5.result().toHex());
    if(it == dataMap->end())
    {
        return nullptr;
    }
    else
    {
        return it.value();
    }
}


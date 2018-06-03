#include "extracthandler.h"

ExtractHandler::ExtractHandler(WebView * webView)
{
    this->webView = webView;
}

ExtractHandler::~ExtractHandler()
{

}

QJsonObject ExtractHandler::handle(QString key, int count)
{
    NetWorkAccessManager * accessManager = webView->getWebPage()->getNetworkAccessManager();
    QMultiMap<QString, QByteArray> * dataMap = accessManager->getExtractMap();
    QJsonArray dataArray;
    QList<QByteArray> values = dataMap->values(key);
    if(values.size() >= count) {
        for(int i = 0; i < values.size(); i ++) {
            dataArray.append(QString(values.at(i)));
        }
        dataMap->remove(key);
    }
    QJsonObject json;
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", dataArray);
    return json;

}


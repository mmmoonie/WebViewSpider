#include "extracthandler.h"

ExtractHandler::ExtractHandler(WebView * webView)
{
    this->webView = webView;
}

ExtractHandler::~ExtractHandler()
{

}

QJsonObject ExtractHandler::handle(QJsonArray keys)
{
    NetWorkAccessManager * accessManager = webView->getWebPage()->getNetworkAccessManager();
    QMap<QString, QByteArray> * dataMap = accessManager->getExtractMap();
    QJsonArray dataArray;
    for(int i = 0; i < keys.size(); i ++)
    {
        QString key = keys.at(i).toString();
        QMap<QString, QByteArray>::iterator it;
        for(it = dataMap->begin(); it != dataMap->end(); it ++) {
            if(it.key().contains(key)) {
                QJsonObject dataJson;
                QJsonArray array;
                QList<QString> values = dataMap->values(it.key());
                foreach(QString val, values) {
                    array.append(val);
                }
                dataJson.insert(key, array);
                dataArray.append(dataJson);
                dataMap->remove(key);
            }
        }
    }
    QJsonObject json;
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", dataArray);
    return json;

}


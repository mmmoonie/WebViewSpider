#include "exechandler.h"

ExecHandler::ExecHandler(WebView * webView)
{
    this->webView = webView;
}

ExecHandler::~ExecHandler()
{

}

QJsonObject ExecHandler::handle(QString &javaScript)
{
    QString tryCatch("try{%1}catch(err){err.toString();}");
    this->webView->getWebPage()->getNetworkAccessManager()->getExtractMap()->clear();
    QVariant val = webView->getWebPage()->mainFrame()->evaluateJavaScript(tryCatch.arg(javaScript));
    QJsonObject json;
    if(val.isNull() || !val.isValid())
    {
        json.insert("code", 200);
        json.insert("desc", "null or undefined");
        json.insert("data", QJsonValue::Null);
    }
    else
    {
        json.insert("code", 200);
        json.insert("desc", "success");
        json.insert("data", val.toString().replace(QRegExp("[\\s\r\n]"), " "));
    }
    return json;
}

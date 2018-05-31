#include "screenshothandler.h"
#include <QWebElement>
#include <QBuffer>

ScreenshotHandler::ScreenshotHandler(WebView * webView)
{
    this->webView = webView;
}

ScreenshotHandler::~ScreenshotHandler()
{

}

QJsonObject ScreenshotHandler::handle(const QString &selector)
{
    QJsonObject json;
    if(selector.isEmpty())
    {
        json.insert("code", 400);
        json.insert("desc", "selector is empty");
        json.insert("data", QJsonValue::Null);
        return json;
    }
    QImage * image = 0;
    QPainter * painter = 0;
    if("html" == selector) {
        image = new QImage(webView->getWebPage()->viewportSize(), QImage::Format_ARGB32);
        painter = new QPainter(image);
        webView->getWebPage()->mainFrame()->render(painter);
    } else {
        QWebElement imgEle = webView->getWebPage()->mainFrame()->findFirstElement(selector);
        if(imgEle.isNull())
        {
            json.insert("code", 400);
            json.insert("desc", "not find");
            json.insert("data", QJsonValue::Null);
            return json;
        }
        image = new QImage(imgEle.geometry().width(), imgEle.geometry().height(), QImage::Format_ARGB32);
        painter = new QPainter(image);
        imgEle.render(painter);
    }
    painter->end();

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image->save(&buffer, "PNG");
    QByteArray hexed = bytes.toBase64();
    buffer.close();

    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QString(hexed));
    return json;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QWebFrame>
#include <QVariant>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QWebElement>
#include <QPainter>
#include <QImage>
#include <QVariant>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>
#include <QMessageBox>
#include <QTimer>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QBuffer>
#include "cookiejar.h"

MainWindow::MainWindow(QString serverName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    localSocket = new QLocalSocket(this);
    localSocket->connectToServer(serverName, QIODevice::ReadWrite);
    if(!localSocket->waitForConnected(3000))
    {
        QTimer::singleShot(0, this, &MainWindow::close);
    }
    else
    {
        if(!localSocket->waitForReadyRead())
        {
            QMessageBox::warning(this, "warn", "wait for read timeout");
            QTimer::singleShot(0, this, &MainWindow::close);
        }
        else
        {
            QByteArray data = localSocket->readAll();
            QJsonParseError * parseError = new QJsonParseError;
            QJsonDocument loadJsonDoc = QJsonDocument::fromJson(data, parseError);
            if(parseError->error != QJsonParseError::NoError || loadJsonDoc.isNull())
            {
                QJsonObject resultJsonObj;
                resultJsonObj.insert("code", 400);
                resultJsonObj.insert("desc", "parse json failed");
                resultJsonObj.insert("data", QJsonValue::Null);
                writeToServer(resultJsonObj);
                QTimer::singleShot(0, this, &MainWindow::close);
            }
            else
            {
                QJsonObject loadJson = loadJsonDoc.object();
                currentOp = loadJson.value("op").toString();
                QUrl url;
                QString interceptor;
                if(currentOp == "load")
                {
                    url = QUrl::fromUserInput(loadJson.value("url").toString("about:blank"));
                    interceptor = loadJson.value("interceptor").toString();
                    QNetworkProxyFactory::setUseSystemConfiguration(true);
                }
                else if(currentOp == "loadWithProxy")
                {
                    url = QUrl::fromUserInput(loadJson.value("url").toString("about:blank"));
                    interceptor = loadJson.value("interceptor").toString();
                    QJsonObject proxyJson = loadJson.value("proxy").toObject();
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
                    QJsonObject resultJsonObj;
                    resultJsonObj.insert("code", 400);
                    resultJsonObj.insert("desc", "first commad must be load or loadWithProxy");
                    resultJsonObj.insert("data", QJsonValue::Null);
                    writeToServer(resultJsonObj);
                }
                ui->setupUi(this);
                webView = new WebView(this);
                webView->getWebPage()->getNetworkAccessManager()->setInterceptor(interceptor);
                connect(localSocket, &QLocalSocket::readyRead, this, &MainWindow::on_localSocket_readyRead);
                connect(webView, &WebView::loadFinished, this, &MainWindow::on_webView_loadFinished);
                webView->load(url);
                setCentralWidget(webView);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_localSocket_readyRead()
{
    QByteArray data = localSocket->readAll();
    QJsonParseError * parseError = new QJsonParseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, parseError);
    if(parseError->error != QJsonParseError::NoError || jsonDoc.isNull())
    {
        QJsonObject resultJsonObj;
        resultJsonObj.insert("code", 400);
        resultJsonObj.insert("desc", "parse json failed");
        resultJsonObj.insert("data", QJsonValue::Null);
        writeToServer(resultJsonObj);
    }
    else
    {
        QJsonObject dataJson = jsonDoc.object();
        QJsonObject resultJsonObj;
        currentOp = dataJson.value("op").toString();
        if(currentOp == "load")
        {
            QUrl url = QUrl::fromUserInput(dataJson.value("url").toString("about:blank"));
            webView->load(url);
        }
        else if(currentOp == "getCookie")
        {
            this->getCookie(resultJsonObj);
            writeToServer(resultJsonObj);
        }
        else if(currentOp == "setCookie")
        {
            QJsonArray cookieArray = dataJson.value("cookies").toArray();
            this->setCookie(resultJsonObj, cookieArray);
            writeToServer(resultJsonObj);
        }
        else if(currentOp == "captcha")
        {
            QString selector = dataJson.value("selector").toString();
            this->captcha(selector, resultJsonObj);
            writeToServer(resultJsonObj);
        }
        else if(currentOp == "exec")
        {
            QString tryCatch("try{%1}catch(err){err.toString();}");
            QString js = dataJson.value("js").toString();
            QVariant val = webView->getWebPage()->mainFrame()->evaluateJavaScript(tryCatch.arg(js));
            if(val.isNull() || !val.isValid())
            {
                resultJsonObj.insert("code", 200);
                resultJsonObj.insert("desc", "null or undefined");
                resultJsonObj.insert("data", QJsonValue::Null);
            }
            else
            {
                resultJsonObj.insert("code", 200);
                resultJsonObj.insert("desc", "success");
                resultJsonObj.insert("data", val.toString().replace(QRegExp("[\\s\r\n]"), " "));
            }
            writeToServer(resultJsonObj);
        }
        else if(currentOp == "execToRedirect")
        {
            QString tryCatch("try{%1}catch(err){err.toString();}");
            QString js = dataJson.value("js").toString();
            webView->getWebPage()->mainFrame()->evaluateJavaScript(tryCatch.arg(js));
        }
        else
        {
            resultJsonObj.insert("code", 400);
            resultJsonObj.insert("desc", "unknow command");
            resultJsonObj.insert("data", QJsonValue::Null);
            writeToServer(resultJsonObj);
        }

    }
}

void MainWindow::on_webView_loadFinished()
{
    QJsonObject resultJsonObj;
    resultJsonObj.insert("code", 200);
    resultJsonObj.insert("desc", "success");
    resultJsonObj.insert("data", webView->url().toString());
    writeToServer(resultJsonObj);
}

void MainWindow::writeToServer(QJsonObject &json)
{
    QJsonDocument resultJsonDoc;
    resultJsonDoc.setObject(json);
    localSocket->write(resultJsonDoc.toJson(QJsonDocument::Compact));
    localSocket->flush();
    localSocket->waitForBytesWritten();
}

void MainWindow::getCookie(QJsonObject &json)
{
    CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
    QJsonArray cookieArray;
    QList<QNetworkCookie> cookieList = cookieJar->getAllCookies();
    for(int i = 0; i < cookieList.size(); i ++)
    {
        QNetworkCookie cookie = cookieList.at(i);
        QJsonObject cookieJson;
        cookieJson.insert("name", QString(cookie.name()));
        cookieJson.insert("value", QString(cookie.value()));
        cookieArray.append(cookieJson);
    }
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue(cookieArray));
}

void MainWindow::setCookie(QJsonObject &json, QJsonArray &cookieArray)
{
    CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
    QList<QNetworkCookie> cookieList = cookieJar->getAllCookies();
    for(int i = 0; i < cookieArray.size(); i ++)
    {
        QJsonObject cookieObj = cookieArray.at(i).toObject();
        const QString name = cookieObj.value("name").toString();
        const QString value = cookieObj.value("value").toString();
        for(int j = 0; j < cookieList.size(); j ++)
        {
            QNetworkCookie originalCookie = cookieList.at(j);
            if(QString(originalCookie.name()) == name)
            {
                originalCookie.setValue(value.toUtf8());
                cookieJar->updateCookie(originalCookie);
            }
        }
    }
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue::Null);
}

void MainWindow::captcha(const QString &selector, QJsonObject &json)
{
    if(selector.isEmpty())
    {
        json.insert("code", 400);
        json.insert("desc", "selector is empty");
        json.insert("data", QJsonValue::Null);
        return;
    }
    QWebElement imgEle = webView->getWebPage()->mainFrame()->findFirstElement(selector);
    if(imgEle.isNull())
    {
        json.insert("code", 400);
        json.insert("desc", "not find");
        json.insert("data", QJsonValue::Null);
        return;
    }
    QImage image(imgEle.geometry().width(), imgEle.geometry().height(), QImage::Format_ARGB32);
    QPainter painter(&image);
    imgEle.render(&painter);
    painter.end();
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QByteArray hexed = bytes.toBase64();
    buffer.close();
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QString(hexed));
}

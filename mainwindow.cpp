#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QWebFrame>
#include <QVariant>
#include <QDebug>
#include <QInputDialog>
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
#include <QJsonArray>
#include "cookiejar.h"

MainWindow::MainWindow(QString serverName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    localSocket = new QLocalSocket(this);
    localSocket->connectToServer(serverName, QIODevice::ReadWrite);
    if(!localSocket->waitForConnected(3000))
    {
        QMessageBox::warning(this, "warn", QString("connect to ").append(serverName).append(" failed"));
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
                QString op = loadJson.value("op").toString();
                QUrl url;
                if(op == "load")
                {
                    url = QUrl::fromUserInput(loadJson.value("url").toString());
                    QNetworkProxyFactory::setUseSystemConfiguration(true);
                }
                else if(op == "loadWithProxy")
                {
                    url = QUrl::fromUserInput(loadJson.value("url").toString());
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
                ui->setupUi(this);
                webView = new WebView(this);
                connect(localSocket, &QLocalSocket::readyRead, this, &MainWindow::on_localSocket_readyRead);
                connect(webView, &WebView::loadFinished, this, &MainWindow::on_webView_loadFinished);
                webView->load(url);

                setCentralWidget(webView);
            }
        }
    }


    //    connect(webView, &QWebView::loadFinished, [=](){
    //        if(step == 1)
    //        {
    //            QWebElement imgEle = webView->getWebPage()->mainFrame()->findFirstElement("img[src=VerifyImageServlet]");
    //            QImage image(imgEle.geometry().width(), imgEle.geometry().height(), QImage::Format_ARGB32);
    //            QPainter painter(&image);
    //            imgEle.render(&painter);
    //            painter.end();
    //            image.save("../captcha.png");

    //            QString captcha = QInputDialog::getText(this, "captcha", "captcha");
    //            QString js = QString("$('input[name=username]').val('%1');")
    //                    .append("$('input[name=password]').val('%2');")
    //                    .append("$('input[name=imagecode]').val('%3');login_submit(loginform);")
    //                    .arg("supermoonie").arg("wangchao123").arg(captcha);
    //            QVariant val = webView->page()->mainFrame()->evaluateJavaScript(js);
    //            qDebug() << val.isValid();
    //            step = 2;
    //        }
    //        else if(step == 2)
    //        {
    //            CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
    //            QList<QNetworkCookie> cookieList = cookieJar->getAllCookies();
    //            for(int i = 0; i < cookieList.size(); i ++)
    //            {
    //                QNetworkCookie cookie = cookieList.at(i);
    //                qDebug() << cookie.name() << cookie.value();
    //            }
    //        }
    //    });
    //    webView->load(QUrl("https://persons.shgjj.com"));
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
        QString op = dataJson.value("op").toString();
        if(op == "getCookie")
        {
            this->getCookie(resultJsonObj);
        }
        resultJsonObj.insert("data", QJsonValue::Null);
        writeToServer(resultJsonObj);
    }
}

void MainWindow::on_webView_loadFinished()
{
    QJsonObject resultJsonObj;
    resultJsonObj.insert("code", 200);
    resultJsonObj.insert("desc", "success");
    resultJsonObj.insert("data", QJsonValue::Null);
    writeToServer(resultJsonObj);
}

void MainWindow::writeToServer(QJsonObject &json)
{
    QJsonDocument resultJsonDoc;
    resultJsonDoc.setObject(json);
    localSocket->write(resultJsonDoc.toJson());
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
        qDebug() << cookie.name() << cookie.value();
        cookieJson.insert("name", QString(cookie.name()));
        cookieJson.insert("value", QString(cookie.value()));
        cookieArray.append(cookieJson);
    }
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue(cookieArray));
}

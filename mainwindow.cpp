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
#include <QToolBar>
#include <QDesktopWidget>
#include <QPrinter>
#include <QMap>
#include <QNetworkInterface>
#include <QList>
#include "cookiejar.h"

MainWindow::MainWindow(int port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
    bool flag = false;
    for(int i = 0; i < nets.count(); i ++)
    {
        if(nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            continue;
        }
        QString macAddress = nets[i].hardwareAddress();
        qDebug() << macAddress;
        if(macAddress.toLower() == "08:6d:41:cc:a1:aa")
        {
            flag = true;
            break;
        }
    }
    if(!flag)
    {
        QMessageBox::warning(this, "warning", "access denied");
        QTimer::singleShot(0, this, &MainWindow::close);
    }
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress::LocalHost, port);
    if(!tcpSocket->waitForConnected(5000))
    {
        QTimer::singleShot(0, this, &MainWindow::close);
    }
    else
    {
        ui->setupUi(this);
        webView = new WebView(this);

        // lineEdit
        locationEdit = new QLineEdit(this);
        locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
        connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::on_locationEdit_returnPressed);

        // toolBar
        QToolBar *toolBar = addToolBar(tr("Navigation"));
        toolBar->addAction(webView->pageAction(QWebPage::Back));
        toolBar->addAction(webView->pageAction(QWebPage::Forward));
        toolBar->addAction(webView->pageAction(QWebPage::Reload));
        toolBar->addAction(webView->pageAction(QWebPage::Stop));
        toolBar->addWidget(locationEdit);

        connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::on_tcpSocket_readyRead);
        connect(webView, &WebView::loadFinished, this, &MainWindow::on_webView_loadFinished);
        connect(webView, &QWebView::loadProgress, this, &MainWindow::on_webView_loadProcess);
        connect(webView, &QWebView::titleChanged, this, &MainWindow::on_webView_titleChanged);
        connect(webView->getWebPage()->getNetworkAccessManager(), &QNetworkAccessManager::sslErrors, [=](QNetworkReply* qnr, const QList<QSslError> & errlist){
            qnr->ignoreSslErrors();
        });
        webView->load(QUrl("about:blank"));
        setCentralWidget(webView);

#ifdef QT_DEBUG
        QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect screenRect = desktopWidget->availableGeometry();
        int width = screenRect.width();
        int height = screenRect.height();
        resize(width/2, height);
        move(0, 0);
#endif
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_locationEdit_returnPressed()
{
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    webView->load(url);
}

void MainWindow::on_tcpSocket_readyRead()
{
    QByteArray data = tcpSocket->readAll();
    QJsonParseError * parseError = new QJsonParseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, parseError);
    if(parseError->error != QJsonParseError::NoError || jsonDoc.isNull())
    {
        QJsonObject resultJsonObj;
        resultJsonObj.insert("code", 400);
        resultJsonObj.insert("desc", "parse json failed");
        resultJsonObj.insert("data", QJsonValue::Null);
        writeToServer(resultJsonObj);
        return;
    }
    QJsonObject dataJson = jsonDoc.object();
    QJsonObject resultJsonObj;
    currentOp = dataJson.value("op").toString();
    QString currentUrl = webView->url().toString();
    if(currentUrl == "about:blank" && currentOp != "load")
    {
        resultJsonObj.insert("code", 400);
        resultJsonObj.insert("desc", "first command must be load");
        resultJsonObj.insert("data", QJsonValue::Null);
        writeToServer(resultJsonObj);
        return;
    }
    if(currentOp == "load")
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
        if(dataJson.contains("extractor"))
        {
            QString extractor = dataJson.value("extractor").toString();
            webView->getWebPage()->getNetworkAccessManager()->setExtractor(extractor);
        }
        QUrl url = QUrl::fromUserInput(dataJson.value("url").toString("about:blank").toLocal8Bit());
        this->progress = 0;
        webView->setUrl(url);
        resultJsonObj.insert("code", 200);
        resultJsonObj.insert("desc", "success");
        resultJsonObj.insert("data", webView->url().toString());
        writeToServer(resultJsonObj);
    }
    else if(currentOp == "progress")
    {
        resultJsonObj.insert("code", 200);
        resultJsonObj.insert("desc", "success");
        resultJsonObj.insert("data", this->progress);
        writeToServer(resultJsonObj);
    }
    else if(currentOp == "extract")
    {

        QJsonArray keys = dataJson.value("extractor").toArray();
        this->extract(keys, resultJsonObj);
        writeToServer(resultJsonObj);
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
    else if(currentOp == "screenshot")
    {
        QString selector = dataJson.value("selector").toString();
        this->screenshot(selector, resultJsonObj);
        writeToServer(resultJsonObj);
    }
    else if(currentOp == "exec")
    {
        if(dataJson.contains("extractor"))
        {
            QString extractor = dataJson.value("extractor").toString();
            webView->getWebPage()->getNetworkAccessManager()->setExtractor(extractor);
        }
        QString tryCatch("try{%1}catch(err){err.toString();}");
        QString js = dataJson.value("js").toString().toLocal8Bit();
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
    else if(currentOp == "printPdf")
    {
        this->printPdf(resultJsonObj);
        writeToServer(resultJsonObj);
    }
    else
    {
        resultJsonObj.insert("code", 400);
        resultJsonObj.insert("desc", "unknow command");
        resultJsonObj.insert("data", QJsonValue::Null);
        writeToServer(resultJsonObj);
    }
}

void MainWindow::on_webView_loadProcess(int progress)
{
    this->progress = progress;
    on_webView_titleChanged();
}

void MainWindow::on_webView_titleChanged()
{
    QString title = webView->title();
    if(title == "")
    {
        title = webView->url().toString();
    }
    if (this->progress <= 0 || this->progress >= 100)
        setWindowTitle(title);
    else
        setWindowTitle(QString("%1 (%2%)").arg(title).arg(this->progress));
}

void MainWindow::on_webView_loadStarted()
{
    this->progress = 0;
}

void MainWindow::on_webView_loadFinished()
{
    this->progress = 100;
    QString currentUrl = webView->url().toString();
    locationEdit->setText(currentUrl);
    webView->setFocus();
}

void MainWindow::writeToServer(QJsonObject &json)
{
    QJsonDocument resultJsonDoc;
    resultJsonDoc.setObject(json);
    QByteArray data = resultJsonDoc.toJson(QJsonDocument::Compact);
    data.append(QString("\r\n"));
    tcpSocket->write(data);
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

void MainWindow::screenshot(const QString &selector, QJsonObject &json)
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

void MainWindow::printPdf(QJsonObject &json)
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName("C:/app/a.pdf");
    webView->print(&printer);
    json.insert("code", 200);
    json.insert("desc", "success");
    json.insert("data", QJsonValue::Null);
}

void MainWindow::extract(const QJsonArray &keys, QJsonObject &json)
{
    QMap<QString, bool> * extractStatusMap = webView->getWebPage()->getNetworkAccessManager()->getExtractStatusMap();
    QMap<QString, QByteArray> * extractMap = webView->getWebPage()->getNetworkAccessManager()->getExtractMap();
    bool ok = true;
    for(int i = 0; i < keys.size(); i ++)
    {
        QString key = keys.at(i).toString();
        if(!extractStatusMap->contains(key) || !extractMap->contains(key))
        {
            ok = false;
        }
    }
    if(ok)
    {
        QJsonArray dataArray;
        for(int i = 0; i < keys.size(); i ++)
        {
            QString key = keys.at(i).toString();
            QMap<QString, QByteArray>::iterator it = extractMap->find(key);
            QJsonObject dataJson;
            dataJson.insert(key, QString(it.value().toBase64()));
            dataArray.append(dataJson);
            extractMap->remove(key);
            extractStatusMap->remove(key);
        }
        json.insert("code", 200);
        json.insert("desc", "success");
        json.insert("data", dataArray);
    }
    else
    {
        json.insert("code", 400);
        json.insert("desc", "not ok");
        json.insert("data", QJsonValue::Null);
    }
}

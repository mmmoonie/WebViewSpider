#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QMessageBox>
#include <QTimer>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QToolBar>
#include <QDesktopWidget>
#include <QPrinter>
#include <QList>
#include "cookiejar.h"
#include "extracthandler.h"
#include "loadhandler.h"
#include "cookiehandler.h"
#include "screenshothandler.h"
#include "exechandler.h"
#include <QNetworkProxyFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any, 7100))
    {
        QMessageBox::warning(this, "warn", "can't listen on 7100");
        QTimer::singleShot(0, this, &MainWindow::close);
        return;
    }
    else
    {
        connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::on_tcpServer_newConnection);
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

        connect(webView, &WebView::loadStarted, this, &MainWindow::on_webView_loadStarted);
        connect(webView, &WebView::loadFinished, this, &MainWindow::on_webView_loadFinished);
        connect(webView, &QWebView::loadProgress, this, &MainWindow::on_webView_loadProcess);
        connect(webView, &QWebView::titleChanged, this, &MainWindow::on_webView_titleChanged);
        connect(webView->getWebPage()->getNetworkAccessManager(), &QNetworkAccessManager::sslErrors, [=](QNetworkReply* qnr, const QList<QSslError> & errlist){
            qnr->ignoreSslErrors();
        });
        webView->load(QUrl("about:blank"));
//        webView->setUrl(QUrl("https://persons.shgjj.com"));
        setCentralWidget(webView);

        QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect screenRect = desktopWidget->availableGeometry();
        int width = screenRect.width();
        int height = screenRect.height();
        resize(width/2, height);
        move(0, 0);

        QNetworkProxyFactory::setUseSystemConfiguration(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tcpServer_newConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::on_tcpSocket_readyRead);
}

void MainWindow::on_tcpSocket_readyRead()
{
    QString data = tcpSocket->readAll();
    qDebug() << data;
    data = data.replace("boundary-----------", "");
    QJsonParseError * parseError = new QJsonParseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8(), parseError);
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
    currentOp = dataJson.value("op").toString();

    if(currentOp == "load")
    {
        QJsonObject json = LoadHandler(webView).handle(dataJson);
        this->progress = 0;
        writeToServer(json);
    }
    else if(currentOp == "progress")
    {
        QJsonObject json;
        json.insert("code", 200);
        json.insert("desc", "success");
        json.insert("data", this->progress);
        writeToServer(json);
    }
    else if(currentOp == "extract")
    {
        QString key = dataJson.value("extractor").toString();
        int count = dataJson.value("count").toInt(1);
        QJsonObject json = ExtractHandler(webView).handle(key, count);
        writeToServer(json);
    }
    else if(currentOp == "getCookie")
    {
        QJsonObject json = CookieHandler(webView).getAllCookies();
        writeToServer(json);
    }
    else if(currentOp == "setCookie")
    {
        QJsonArray cookieArray = dataJson.value("cookies").toArray();
        QJsonObject json = CookieHandler(webView).setCookies(cookieArray);
        writeToServer(json);
    }
    else if(currentOp == "deleteCookie")
    {
        QString url = dataJson.value("url").toString("");
        QJsonObject json = CookieHandler(webView).deleteCookiesFromUrl(url);
        writeToServer(json);
    }
    else if(currentOp == "screenshot")
    {
        QString selector = dataJson.value("selector").toString();
        QJsonObject json = ScreenshotHandler(webView).handle(selector);
        writeToServer(json);
    }
    else if(currentOp == "exec")
    {
        QString js = dataJson.value("js").toString().toLocal8Bit();
        QJsonObject json = ExecHandler(webView).handle(js);
        writeToServer(json);
    }
    else if(currentOp == "printPdf")
    {
        QJsonObject json;
        this->printPdf(json);
        writeToServer(json);
    }
    else
    {
        QJsonObject json;
        json.insert("code", 400);
        json.insert("desc", "unknow command");
        json.insert("data", QJsonValue::Null);
        writeToServer(json);
    }
}

void MainWindow::on_locationEdit_returnPressed()
{
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    webView->setUrl(url);
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
    this->webView->getWebPage()->getNetworkAccessManager()->getExtractMap()->clear();
    locationEdit->setText(webView->url().toString());
}

void MainWindow::on_webView_loadFinished()
{
    this->progress = 100;
    on_webView_titleChanged();
    QWebSettings::clearMemoryCaches();
    locationEdit->setText(webView->url().toString());
}

void MainWindow::writeToServer(QJsonObject &json)
{
    QJsonDocument resultJsonDoc;
    resultJsonDoc.setObject(json);
    QByteArray data = resultJsonDoc.toJson(QJsonDocument::Compact);
    data.append(QString("\r\n"));
    data.append(QString("boundary-----------"));
    data.append(QString("\r\n"));
    tcpSocket->write(data);
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

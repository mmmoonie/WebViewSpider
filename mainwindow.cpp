#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QWebFrame>
#include <QVariant>
#include <QDebug>
#include <QInputDialog>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QNetworkProxyFactory>
#include <QWebElement>
#include <QPainter>
#include <QImage>
#include <QVariant>
#include "cookiejar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    webView = new WebView(this);
    connect(webView, &QWebView::loadFinished, [=](){
        if(step == 1)
        {
            QWebElement imgEle = webView->getWebPage()->mainFrame()->findFirstElement("img[src=VerifyImageServlet]");
            QImage image(imgEle.geometry().width(), imgEle.geometry().height(), QImage::Format_ARGB32);
            QPainter painter(&image);
            imgEle.render(&painter);
            painter.end();
            image.save("../captcha.png");

            QString captcha = QInputDialog::getText(this, "captcha", "captcha");
            QString js = QString("$('input[name=username]').val('%1');")
                    .append("$('input[name=password]').val('%2');")
                    .append("$('input[name=imagecode]').val('%3');login_submit(loginform);")
                    .arg("").arg("").arg(captcha);
            QVariant val = webView->page()->mainFrame()->evaluateJavaScript(js);
            qDebug() << val.isValid();
            step = 2;
        }
        else if(step == 2)
        {
            CookieJar * cookieJar = webView->getWebPage()->getNetworkAccessManager()->getCookieJar();
            QList<QNetworkCookie> cookieList = cookieJar->getAllCookies();
            for(int i = 0; i < cookieList.size(); i ++)
            {
                QNetworkCookie cookie = cookieList.at(i);
                qDebug() << cookie.name() << cookie.value();
            }
        }
    });
    webView->load(QUrl("https://persons.shgjj.com"));
    setCentralWidget(webView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

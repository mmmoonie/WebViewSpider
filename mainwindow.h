#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include "webview.h"
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void on_tcpServer_newConnection();
    void on_tcpSocket_readyRead();
    void on_webView_loadStarted();
    void on_webView_loadFinished();
    void on_locationEdit_returnPressed();
    void on_webView_titleChanged();
    void on_webView_loadProcess(int progress);

public slots:


private:
    Ui::MainWindow *ui;
    QLineEdit * locationEdit;
    WebView * webView;
    QTcpServer * tcpServer;
    QTcpSocket * tcpSocket;
    QString currentOp;
    int progress = 0;
    void writeToServer(QJsonObject &json);
    void getCookie(QJsonObject &json);
    void setCookie(QJsonObject &json, QJsonArray &cookieArray);
    void screenshot(const QString &selector, QJsonObject &json);
    void printPdf(QJsonObject &json);
    void extract(const QJsonArray &keys, int count, QJsonObject &json);
};

#endif // MAINWINDOW_H

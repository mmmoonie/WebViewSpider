#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocalSocket>
#include <QJsonObject>
#include "webview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString serverName, QWidget *parent = 0);
    ~MainWindow();
    void on_localSocket_readyRead();
    void on_webView_loadFinished();

public slots:


private:
    Ui::MainWindow *ui;
    WebView * webView;
    QLocalSocket * localSocket;
    void writeToServer(QJsonObject &json);
    void getCookie(QJsonObject &json);

};

#endif // MAINWINDOW_H

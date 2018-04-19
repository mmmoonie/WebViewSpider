#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList args = QApplication::arguments();
    foreach(QString arg, args)
    {
        qDebug() << arg;
    }
    QString serverName = QString("WebViewSpider");
    if(args.size() == 2)
    {
        serverName = args[1];
    }
    MainWindow w(serverName);
#ifdef QT_NO_DEBUG
    w.showMinimized();
#else
    w.showMaximized();
#endif

    return app.exec();
}

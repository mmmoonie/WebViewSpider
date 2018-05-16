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
    int port = 7200;
    if(args.size() == 2)
    {
        port = args[1].toInt();
    }
    MainWindow w(port);
    w.showMinimized();

    return app.exec();
}

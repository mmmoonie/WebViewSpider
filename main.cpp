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
    MainWindow w;
    w.show();

    return app.exec();
}

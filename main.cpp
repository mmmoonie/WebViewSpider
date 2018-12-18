#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    qDebug() << cachePath;
    MainWindow w;
    w.show();

    return app.exec();
}

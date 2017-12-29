#include "mainwindow.h"
#include <QApplication>
#include "NetworkInterface.h"
#include <QDebug>
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

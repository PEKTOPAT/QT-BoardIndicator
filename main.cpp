#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << "argc" << argc;
    qDebug() << "argv" << argv[0];
     qDebug() << "argv" << argv[0];
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

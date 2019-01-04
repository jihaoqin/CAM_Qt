#include "mainwindow.h"
#include <QApplication>
#include "glwidget.h"
#include "Mesh.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Mesh m;

    return a.exec();
}

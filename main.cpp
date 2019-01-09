#include "mainwindow.h"
#include <QApplication>
#include "glwidget.h"
#include "Model.h"
#include "Line.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

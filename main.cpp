#include "mainwindow.h"
#include "Data.h"
#include "Controller.h"
#include <QApplication>
#include "glwidget.h"
#include "test_only.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::shared_ptr<Data> data = std::make_shared<Data>();
    Controller ctrl;
    ctrl.bindData(data);
    w.bindController(&ctrl);
    w.show();

    MainTest();
    return a.exec();
}

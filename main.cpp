#include "mainwindow.h"
#include "Data.h"
#include "Controller.h"
#include <QApplication>
#include "glwidget.h"
#include "test_only.h"
#include <fstream>
#include "GuiConnector.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<Data> data = std::make_shared<Data>();
    Controller ctrl;
    ctrl.bindData(data);
    MainWindow w(&ctrl);
    data->bindConnector(w.connector);
    w.show();
    ctrl.addTee(90, 69, 7.5, 7.5);
    //ctrl.addTee(90, 52.5, 7.5, 7.5);
    //ctrl.addTee(90, 37.5, 7.5, 7.5);
    //ctrl.addTee(60, 27.5, 7.5, 7.5);//环绕模具
    data->bandWidth(2);
    data->getEnvelopeIni() = EnvelopData{40, 20, 5};
    return a.exec();
}

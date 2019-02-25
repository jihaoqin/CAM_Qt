#include "mainwindow.h"
#include "Data.h"
#include "Controller.h"
#include <QApplication>
#include "glwidget.h"
#include "test_only.h"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include "GuiConnector.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<Data> data = std::make_shared<Data>();
    Controller ctrl;
    ctrl.bindData(data);
    MainWindow w(&ctrl);
    w.show();
    ctrl.addTee(100, 100, 10, 10);
    ctrl.addCylinder(glm::vec3(0, 0, 0), glm::vec3(0, 0, -100), 25);
    ctrl.addRing(40, 20, utility::PI/2,glm::vec3(0,0,0), glm::vec3(0,0,1),glm::vec3(1,0,0));
    return a.exec();
}

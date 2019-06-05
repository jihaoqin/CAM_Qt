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
    //begin 石膏三通管模型
    ctrl.addTee(90, 69, 7.5, 7.5);
    //ctrl.addTee(90, 52.5, 7.5, 7.5);
    //ctrl.addTee(80, 50, 7.5, 7.5);//主管一段-5，支管-12.5
    //ctrl.addTee(65, 40, 7.5, 7.5);//
    //ctrl.addTee(70, 30, 7.5, 7.5);//
    //ctrl.addTee(50, 50, 7.5, 7.5);//
    //ctrl.addTee(50, 60, 7.5, 7.5);//
    //data->getEnvelopeIni() = EnvelopData{40, 20, 5};
    //end 石膏三通管模型

    //begin 机器人三通管
    //实际尺寸 	ctrl.addTee(440, 230, 54, 25);
    ctrl.addTee(420, 220, 54, 25);
    //end 机器人三通管
    data->bandWidth(6);
    data->getEnvelopeIni() = EnvelopData{90, 60, 5};
    return a.exec();
}

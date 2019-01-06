#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include "glwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),model("F:\\0qin\\Qt\\CAM_Qt\\3.STL")
{
    QMenuBar* menuBar = new QMenuBar();
    QMenu* menu = new QMenu("File");
    menuBar->addMenu(menu);
    setMenuBar(menuBar);
    setCentralWidget(&widget);
    resize(800,600);
    //widget.bindGL(model);
}

MainWindow::~MainWindow()
{

}

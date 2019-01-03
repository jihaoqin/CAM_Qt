#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include "glwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar* menuBar = new QMenuBar();
    QMenu* menu = new QMenu("File");
    menuBar->addMenu(menu);
    setMenuBar(menuBar);
    GLWidget widget;
    setCentralWidget(&widget);
}

MainWindow::~MainWindow()
{

}

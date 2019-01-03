#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar* menuBar = new QMenuBar();
    QMenu* menu = new QMenu("File");
    menuBar->addMenu(menu);
    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{

}

#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>
#include "glwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    configureMenuBar();
    configureStatusBar();
    setCentralWidget(&widget);
    resize(800, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::bindController(Controller *c){
    widget.bindController(c);
}

void MainWindow::configureStatusBar(){
    QStatusBar* staBar = statusBar();
    QLabel *label = new QLabel(this);
    label->setText("No File");
    staBar->addPermanentWidget(label);

}

void MainWindow::configureMenuBar(){
    QMenuBar* menubar = menuBar();
    QMenu* menu = new QMenu("File");
    menubar->addMenu(menu);
    QAction* actionNew = menu->addAction("New");
    QAction* actionOpen = menu->addAction("Open");
    QAction* actionSave = menu->addAction("Save");
}

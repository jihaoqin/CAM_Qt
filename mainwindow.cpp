#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>
#include "glwidget.h"
#include "TeeParameterDialog.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ctrl(nullptr)
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
    ctrl = c;
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

    //configure New operation
    connect(actionNew, &QAction::triggered, this, &MainWindow::saveOrNot);
}
void MainWindow::saveOrNot(){
    if(ctrl->hasTee()){
        //弹出说明是否保存
        int ret =QMessageBox::warning(this,"Warning",tr("Save changes to document?"),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Save);
        switch(ret){
            case QMessageBox::Save:
                //保存data,之后弹出tee参数化对话框
                break;
            case QMessageBox::Discard:
                //不保存data, 之后弹出tee参数化对话框
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }
    else{
        showTeeParameterDialog();
    }
}

void MainWindow::showTeeParameterDialog(){
    teeNewDialog = new TeeParameterDialog(this);
    connect(teeNewDialog, &TeeParameterDialog::addTee, &widget, &GLWidget::addTee);
    teeNewDialog->exec();
    delete teeNewDialog;
}

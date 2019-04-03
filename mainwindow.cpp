#include "mainwindow.h"
#include <QList>
#include <QToolBar>
#include <QString>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>
#include "TeeParameterDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include "CentralWidget.h"
#include "GuiConnector.h"
#include <QFileDialog>

MainWindow::MainWindow(Controller* c, QWidget *parent)
    : QMainWindow(parent), ctrl(nullptr), widget(nullptr), teeNewDialog(nullptr), connector(nullptr)
{
    //setStyleSheet("background-color:rgb(255,255,255)");
    widget = new CentralWidget(this);
    configureMenuBar();
    configureStatusBar();
    configureToolBar();
    setMouseTracking(true);
    setCentralWidget(widget);
    centralWidget()->setMouseTracking(true);
    bindController(c);
    connector = new GuiConnector;
    connector->setRoot(this);
    widget->setConnector(connector);
    //resize(800, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::bindController(Controller *c){
    widget->bindController(c);
    ctrl = c;
    ctrl->bindMainWindow(this);
}

void MainWindow::configureStatusBar(){
    QStatusBar* staBar = statusBar();
    QLabel *label = new QLabel(this);
    label->setText("No File");
    staBar->addPermanentWidget(label);

}

void MainWindow::configureMenuBar(){
    QMenuBar* menubar = menuBar();
    fileMenu = new QMenu("File",this);
    menubar->addMenu(fileMenu);
    actionNew = fileMenu->addAction("New");
    actionOpen = fileMenu->addAction("Open");
    actionSave = fileMenu->addAction("Save");

    bandMenu = new QMenu("Band", this);
    menubar->addMenu(bandMenu);
    bandSave = bandMenu->addAction("Save");
    bandOpen = bandMenu->addAction("Open");

    //update the status of actions below "File" menu
    connect(fileMenu, &QMenu::aboutToShow, this, &MainWindow::updateAction);
    connect(actionNew, &QAction::triggered, this, &MainWindow::saveOrNot);
    connect(bandMenu, &QMenu::aboutToShow, this, &MainWindow::updateAction);
    connect(bandSave, &QAction::triggered, this, &MainWindow::saveBand);
    connect(bandOpen, &QAction::triggered, this, &MainWindow::openBand);
}
void MainWindow::saveOrNot(){
    if(ctrl->hasTee()){
        //弹出说明是否保存
        int ret =QMessageBox::warning(this,"Warning",tr("Save changes to document?"),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Save);
        switch(ret){
        case QMessageBox::Save:{
            //保存data,之后弹出tee参数化对话框
            QString savePath = QFileDialog::getSaveFileName();
            if(savePath.isNull()){
                //do nothing
            }
            else{
                ctrl->save(savePath);
            }
            ctrl->clearData();
            showTeeParameterDialog();
            break;
        }
        case QMessageBox::Discard:{
            //不保存data, 之后弹出tee参数化对话框
            ctrl->clearData();
            showTeeParameterDialog();
            break;
        }
        case QMessageBox::Cancel:{
            //do nothing
            break;
        }
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
    int flag = teeNewDialog->exec();
    if(flag){
        TeePara para = teeNewDialog->getTeePara();
        ctrl->addTee(para.mainLength, para.branchLength, para.R, para.sideR);
        updateAction();
    }
    delete teeNewDialog;
}

void MainWindow::updateAction(){
    bool opFlag = false;
    if(widget->isOperating()){
        opFlag = true;
    }
    QList<QAction*> actions = fileMenu->actions();
    for(auto action : actions){
        QString s = action->text();
        if(s == QString("Save")){
            bool empty = ctrl->getEmpty();
            bool changed = ctrl->getChanged();
            if(empty == true){
                action->setEnabled(false);
            }
            else{
                if(changed == true){
                    action->setEnabled(true);
                }
                else{
                    action->setEnabled(false);
                }
            }
            if(true == opFlag){
                action->setEnabled(false);
            }
            else{
                //do nothing
            }
        }
        else if(s == QString("New")){
            if(true == opFlag){
                action->setEnabled(false);
            }
            else{
                action->setEnabled(true);
            }
        }
        else if(s == QString("Open")){
            if(true == opFlag){
                action->setEnabled(false);
            }
            else{
                action->setEnabled(true);
            }
        }
    }

    //toolBar
    QList<QAction*> actionsTool = toolBar->actions();
    for(auto action:actionsTool){
        QString s = action->text();
        if(s == QString("newCurve")){
            bool empty = ctrl->getEmpty();
            if(empty == true){
                action->setEnabled(false);
            }
            else{
                action->setEnabled(true);
            }
            if(true == opFlag){
                action->setEnabled(false);
            }
        }
    }

    {
        bool hasT = ctrl->hasTee();
        if(hasT){
            bandSave->setEnabled(true);
            bandOpen->setEnabled(true);
        }
        else{
            bandSave->setEnabled(false);
            bandOpen->setEnabled(false);
        }
        if(true == opFlag){
            bandSave->setEnabled(false);
            bandOpen->setEnabled(false);
        }
    }
}

void MainWindow::configureToolBar(){
    toolBar = addToolBar("newCurve");
    toolBar->setIconSize(QSize(40,40));
    newCurve = new QAction(QIcon(":/icons/newCurve"),"newCurve");
    newCurve->setEnabled(false);
    toolBar->addAction(newCurve);
    connect(newCurve, &QAction::triggered, this, &MainWindow::showNewCurveTab);
    leftCurve = new QAction(QIcon(":/icons/leftCurve"), "leftCurve");
    toolBar->addAction(leftCurve);
    leftCurve->setEnabled(true);
    connect(leftCurve, &QAction::triggered, this, &MainWindow::genLeftCurve);
    upCurve = new QAction(QIcon(":/icons/upCurve"), "upCurve");
    toolBar->addAction(upCurve);
    upCurve->setEnabled(false);
    rightCurve = new QAction(QIcon(":/icons/rightCurve"), "rightCurve");
    toolBar->addAction(rightCurve);
    rightCurve->setEnabled(false);
}

void MainWindow::showNewCurveTab(){
    widget->showNewCurveTab();
    updateAction();
}

void MainWindow::saveBand(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save Curve","","*.curve");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        ctrl->saveBand(fileName);
    }
}

void MainWindow::openBand(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open Curve","","*.curve");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        ctrl->openBand(fileName);
    }
}

void MainWindow::genLeftCurve(){
    ctrl->genLeftCurve();
}

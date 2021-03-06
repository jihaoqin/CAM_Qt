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
#include "GenCurveProgressDialog.h"
#include "OpenBandThread.h"
#include "GenGeneralBandThread.h"
#include "ClosePathDialog.h"
#include "ClosePathThread.h"
#include "AnimateController.h"
#include "SimulationTab.h"
#include <QTimer>
#include "Data.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>
#include "Node.h"

using namespace rapidjson;

MainWindow::MainWindow(Controller* c, QWidget *parent)
    : QMainWindow(parent), ctrl(nullptr), widget(nullptr), teeNewDialog(nullptr), connector(nullptr)
{
    //setStyleSheet("background-color:rgb(255,255,255)");
    setWindowTitle("FiberStudio");
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
    //QLabel *label = new QLabel(this);
    //label->setText("No File");
    //staBar->addPermanentWidget(label);

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
    freeze = bandMenu->addAction("freeze");

    //update the status of actions below "File" menu
    connect(fileMenu, &QMenu::aboutToShow, this, &MainWindow::updateAction);
    connect(actionNew, &QAction::triggered, this, &MainWindow::saveOrNot);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveIni);
    connect(bandMenu, &QMenu::aboutToShow, this, &MainWindow::updateAction);
    connect(bandSave, &QAction::triggered, this, &MainWindow::saveBand);
    connect(bandOpen, &QAction::triggered, this, &MainWindow::openBand);
    connect(freeze, &QAction::triggered, this, &MainWindow::freezeBand);
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
                //ctrl->save(savePath);
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
        float width = teeNewDialog->bandWidth();
        ctrl->getData()->bandWidth(width);
        ctrl->addTee(para.lengthMain, para.lengthBranch, para.pipeR, para.sideR);
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
            leftCurve->setEnabled(true);
            upCurve->setEnabled(true);
            rightCurve->setEnabled(true);
            loopCurve->setEnabled(true);
            simAction->setEnabled(true);
            fromPipeCurve->setEnabled(true);
        }
        else{
            leftCurve->setEnabled(false);
            upCurve->setEnabled(false);
            rightCurve->setEnabled(false);
            loopCurve->setEnabled(false);
            simAction->setEnabled(false);
            fromPipeCurve->setEnabled(false);
        }
        if(true == opFlag){
            leftCurve->setEnabled(false);
            upCurve->setEnabled(false);
            rightCurve->setEnabled(false);
            loopCurve->setEnabled(false);
            simAction->setEnabled(false);
            fromPipeCurve->setEnabled(false);
        }
    }

    //menuBand
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

    fromPipeCurve = new QAction(QIcon(":/icons/fromPipeCurve"), "fromPipeCurve");
    fromPipeCurve->setEnabled(false);
    toolBar->addAction(fromPipeCurve);
    connect(fromPipeCurve, &QAction::triggered, this, &MainWindow::showFromPipeCurveTab);

    leftCurve = new QAction(QIcon(":/icons/leftCurve"), "leftCurve");
    toolBar->addAction(leftCurve);
    leftCurve->setEnabled(true);
    connect(leftCurve, &QAction::triggered, this, &MainWindow::genLeftCurve);

    upCurve = new QAction(QIcon(":/icons/upCurve"), "upCurve");
    toolBar->addAction(upCurve);
    upCurve->setEnabled(false);
    connect(upCurve, &QAction::triggered, this, &MainWindow::genUpCurve);

    rightCurve = new QAction(QIcon(":/icons/rightCurve"), "rightCurve");
    toolBar->addAction(rightCurve);
    rightCurve->setEnabled(false);
    connect(rightCurve, &QAction::triggered, this, &MainWindow::genRightCurve);

    loopCurve = new QAction(QIcon(":/icons/loop"), "loopCurve");
    toolBar->addAction(loopCurve);
    loopCurve->setEnabled(false);
    connect(loopCurve, &QAction::triggered, this, &MainWindow::loopBand);

    simAction = new QAction(QIcon(":/icons/sim"),"simulation");
    toolBar->addAction(simAction);
    simAction->setEnabled(false);
    connect(simAction, &QAction::triggered, this, &MainWindow::showSimTab);


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
        GenCurveProgressDialog* dlg = new GenCurveProgressDialog(0, 0, this);
        OpenBandThread *thread = new OpenBandThread(fileName, connector->getCtrl());
        connect(thread,&OpenBandThread::progress, dlg, &GenCurveProgressDialog::setData);
        connect(thread,&OpenBandThread::calOver, this, &MainWindow::updateModel);
        connect(thread,&OpenBandThread::calOver, dlg, &GenCurveProgressDialog::close);
        connect(thread,&OpenBandThread::calOver, this, &MainWindow::allBind);
        connect(thread, &OpenBandThread::finished, thread, &OpenBandThread::deleteLater);
        thread->start();
        dlg->exec();
        //delete dlg;
    }
}

void MainWindow::allBind(){
    ctrl->allBindGL();
}

void MainWindow::genLeftCurve(){
    GenCurveProgressDialog* dlg = new GenCurveProgressDialog(0, 0, this);
    GenGeneralBandThread* thread = new GenGeneralBandThread("left", connector->getCtrl());
    connect(thread,&GenGeneralBandThread::progress, dlg, &GenCurveProgressDialog::setData);
    connect(thread,&GenGeneralBandThread::calOver, this, &MainWindow::updateModel);
    connect(thread,&GenGeneralBandThread::calOver, dlg, &GenCurveProgressDialog::close);
    connect(thread,&GenGeneralBandThread::calOver, this, &MainWindow::allBind);
    connect(thread, &OpenBandThread::finished, thread, &OpenBandThread::deleteLater);
    thread->start();
    dlg->exec();
    //delete dialog;
}

void MainWindow::genUpCurve(){
    GenCurveProgressDialog* dlg = new GenCurveProgressDialog(0, 0, this);
    GenGeneralBandThread* thread = new GenGeneralBandThread("up", connector->getCtrl());
    connect(thread,&GenGeneralBandThread::progress, dlg, &GenCurveProgressDialog::setData);
    connect(thread,&GenGeneralBandThread::calOver, this, &MainWindow::updateModel);
    connect(thread,&GenGeneralBandThread::calOver, dlg, &GenCurveProgressDialog::close);
    connect(thread,&GenGeneralBandThread::calOver, this, &MainWindow::allBind);
    connect(thread, &OpenBandThread::finished, thread, &OpenBandThread::deleteLater);
    thread->start();
    dlg->exec();
    //delete dialog;
}

void MainWindow::genRightCurve(){
    GenCurveProgressDialog* dlg = new GenCurveProgressDialog(0, 0, this);
    GenGeneralBandThread* thread = new GenGeneralBandThread("right", connector->getCtrl());
    connect(thread,&GenGeneralBandThread::progress, dlg, &GenCurveProgressDialog::setData);
    connect(thread,&GenGeneralBandThread::calOver, this, &MainWindow::updateModel);
    connect(thread,&GenGeneralBandThread::calOver, dlg, &GenCurveProgressDialog::close);
    connect(thread,&GenGeneralBandThread::calOver, this, &MainWindow::allBind);
    connect(thread, &OpenBandThread::finished, thread, &OpenBandThread::deleteLater);
    thread->start();
    dlg->exec();
    //delete dialog;
}

void MainWindow::updateModel(){
    connector->updateModel();
}

void MainWindow::loopBand(){
    ClosePathDialog* dlg = new ClosePathDialog(this);
    ClosePathThread* thread = new ClosePathThread(connector->getCtrl());
    connect(thread, &ClosePathThread::progress, dlg, &ClosePathDialog::setData);
    connect(thread, &ClosePathThread::calOver, this, &MainWindow::updateModel);
    connect(thread, &ClosePathThread::calOver, dlg, &ClosePathDialog::close);
    connect(thread, &ClosePathThread::calOver, this, &MainWindow::allBind);
    connect(thread, &OpenBandThread::finished, thread, &OpenBandThread::deleteLater);
    thread->start();
    dlg->exec();
    //updateAction()?
}

void MainWindow::animationOver(){
    auto simTab = connector->getSimTab();
    simTab->pause();
}

void MainWindow::showSimTab(){
    TabWidget* t = connector->getTabWidget();
    t->showSimulationTab();
    updateAction();
}


void MainWindow::showFromPipeCurveTab(){
    TabWidget* t = connector->getTabWidget();
    t->showFromPipeCurveTab();
    updateAction();
}


void MainWindow::saveIni(){
    QString fileName = QFileDialog::getSaveFileName(this, "save","","*.windIni");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        ctrl->getData()->serialize(writer);
        std::ofstream outFile;
        outFile.open(fileName.toLatin1().data());
        outFile<<sb.GetString();
        outFile.close();
    }
}

void MainWindow::freezeBand(){
    auto children = ctrl->getData()->getNodeRoot()->childrenPtrVec();
    for(auto c:children){
        DataObjectPtr objPtr = c->getData();
        QString name = objPtr->getId();
        if(name.contains("band")){
            QString ind = name.split('d').at(1);
            QString newName = QString("Band")+ind;
            //const char* newName = (QString("Band")+ind).toLatin1().data();
            auto bandPtr = std::dynamic_pointer_cast<Band>(objPtr);
            bandPtr->setId(newName.toLatin1().data());
        }
    }
}

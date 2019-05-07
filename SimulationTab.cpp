#include "SimulationTab.h"
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "AnimateController.h"
#include "GuiConnector.h"
#include "TabBackground.h"
#include <QTimer>
#include <QCheckBox>
#include "EnvelopIniDialog.h"
#include <QLabel>
#include "Data.h"
#include "AxisConfigDialog.h"
#include <QFileDialog>
#include <fstream>
#include "HangingBandSet.h"
#include "Node.h"
#include "Model.h"

SimulationTab::SimulationTab(TabBackground* back, GuiConnector* con, QWidget* parent)
    :QWidget(parent), backWidget(back), connector(con)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QFont title;
    title.setPointSize(12);
    machineLabel = new QLabel(this);
    machineLabel->setFont(title);

    QFont content;
    content.setPointSize(10);
    axis_1_nameLabel = new QLabel(this);
    axis_2_nameLabel = new QLabel(this);
    axis_3_nameLabel = new QLabel(this);
    axis_4_nameLabel = new QLabel(this);
    axis_5_nameLabel = new QLabel(this);
    axis_1_nameLabel->setFont(content);
    axis_2_nameLabel->setFont(content);
    axis_3_nameLabel->setFont(content);
    axis_4_nameLabel->setFont(content);
    axis_5_nameLabel->setFont(content);

    layout->addWidget(machineLabel);
    layout->addWidget(axis_1_nameLabel);
    layout->addWidget(axis_2_nameLabel);
    layout->addWidget(axis_3_nameLabel);
    layout->addWidget(axis_4_nameLabel);
    layout->addWidget(axis_5_nameLabel);
    updateLabel();

    outputButton = new QPushButton("export G-code", this);
    outputButton->setEnabled(false);
    connect(outputButton, &QPushButton::clicked, this, &SimulationTab::output);
    axisIniButton = new QPushButton("axis ini", this);
    layout->addWidget(axisIniButton);
    connect(axisIniButton, &QPushButton::clicked, this, &SimulationTab::modifyMachine);
    envelopButton = new QPushButton("envelop ini", this);
    calButton = new QPushButton("calculation", this);
    absoluteBox = new QCheckBox("absolute", this);
    hangVisableBox = new QCheckBox("show hang lines", this);
    hangVisableBox->setChecked(true);
    playAndPauseButton = new QPushButton(QIcon(":/icons/play"), "", this);
    playAndPauseButton ->setEnabled(false);
    slowButton = new QPushButton(QIcon(":/icons/sf"),"", this);
    slowButton->setEnabled(false);
    closeButton = new QPushButton("close", this);
    closeButton->setEnabled(true);
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setEnabled(false);
    progressSlider->setRange(0,100);
    fastButton = new QPushButton(QIcon(":/icons/ff"),"", this);
    fastButton->setEnabled(false);
    fFrameButton = new QPushButton(QIcon(":/icons/fFrame"),"", this);
    fFrameButton->setEnabled(false);
    bFrameButton = new QPushButton(QIcon(":/icons/bFrame"), "", this);
    bFrameButton->setEnabled(false);
    animateCtrl = new AnimateController(connector->getCtrl());
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(slowButton);
    hLayout->addWidget(bFrameButton);
    hLayout->addWidget(playAndPauseButton);
    hLayout->addWidget(fFrameButton);
    hLayout->addWidget(fastButton);
    layout->addWidget(envelopButton);
    layout->addWidget(calButton);
    layout->addWidget(absoluteBox);
    layout->addWidget(hangVisableBox);
    layout->addLayout(hLayout);
    layout->addWidget(progressSlider);
    layout->addWidget(outputButton);
    layout->addWidget(closeButton);
    layout->addStretch(1);
    setLayout(layout);
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(envelopButton, &QPushButton::clicked, this, &SimulationTab::showEnvelopIniDialog);
    connect(calButton, &QPushButton::clicked, this, &SimulationTab::calculation);
    connect(playAndPauseButton, &QPushButton::clicked, this, &SimulationTab::playOrPause);
    connect(slowButton, &QPushButton::clicked, this, &SimulationTab::slowForward);
    connect(closeButton, &QPushButton::clicked, this, &SimulationTab::closeMyself);
    connect(fastButton, &QPushButton::clicked, this, &SimulationTab::fastForward);
    connect(progressSlider, &QSlider::valueChanged, this, &SimulationTab::setPercent);
    connect(fFrameButton, &QPushButton::clicked, this, &SimulationTab::nextFrame);
    connect(bFrameButton, &QPushButton::clicked, this, &SimulationTab::lastFrame);
    connect(timer, &QTimer::timeout, this, &SimulationTab::showNext);
}

void SimulationTab::calculation(){
    animateCtrl->calculation();
    calMoveData();
    setHeadAnimate();
    if(!animateCtrl->hasData()){
        return;
    }
    playAndPauseButton->setEnabled(true);
    slowButton->setEnabled(true);
    progressSlider->setEnabled(true);
    fastButton->setEnabled(true);
    fFrameButton->setEnabled(true);
    bFrameButton->setEnabled(true);
    outputButton->setEnabled(true);
}

void SimulationTab::playOrPause(){
    if(timer->isActive()){
        timer->stop();
        playAndPauseButton->setIcon(QIcon(":/icons/play"));
    }
    else{
        timer->start();
        playAndPauseButton->setIcon(QIcon(":/icons/pause"));
    }
}

void SimulationTab::showNext(){
    animateCtrl->showNext();
    int percent = animateCtrl->getPercent();
    progressSlider->blockSignals(true);
    progressSlider->setValue(percent);
    progressSlider->blockSignals(false);
}

void SimulationTab::closeMyself(){
    timer->stop();
    animateCtrl->resetBand();
    animateCtrl->hideBandSet();
    animateCtrl->resetAnimateT();
    backWidget->setWidget(nullptr);
    close();
}

void SimulationTab::setPercent(int percent){
    animateCtrl->setPercent(percent);
}

void SimulationTab::fastForward(){
    int  intl = timer->interval();
    int nextIntl = intl/2;
    if(nextIntl<=0){
        nextIntl = 1;
    }
    timer->setInterval(nextIntl);
}


void SimulationTab::pause(){
    timer->stop();
    playAndPauseButton->setIcon(QIcon(":/icons/play"));
}


void SimulationTab::slowForward(){
    int  intl = timer->interval();
    int nextIntl = intl*2;
    timer->setInterval(nextIntl);
}

void SimulationTab::nextFrame(){
    showNext();
}

void SimulationTab::lastFrame(){

}

bool SimulationTab::isAbsolute(){
    return absoluteBox->isChecked();
}


bool SimulationTab::shallShowHangLines(){
    return hangVisableBox->isChecked();
}


void SimulationTab::showEnvelopIniDialog(){
    EnvelopIniDialog dialog(connector, this);
    dialog.exec();
}

void SimulationTab::updateLabel(){
    auto& axiss = connector->getData()->getAxissIni();
    machineLabel->setText(axiss.tabMachineName());

    axis_1_nameLabel->setText(axiss.tabAxis(0));
    axis_2_nameLabel->setText(axiss.tabAxis(1));
    axis_3_nameLabel->setText(axiss.tabAxis(2));
    axis_4_nameLabel->setText(axiss.tabAxis(3));
    if(axiss.axisSum() == 4){
        axis_5_nameLabel->hide();
    }
    else{
        axis_5_nameLabel->show();
        axis_5_nameLabel->setText(axiss.tabAxis(4));
    }
}

void SimulationTab::modifyMachine(){
    auto& axisIni = connector->getData()->getAxissIni();
    AxisConfigDialog* dialog = new AxisConfigDialog(connector, &axisIni, this);
    dialog->exec();
    updateLabel();
}

void SimulationTab::output(){
    //calMoveData();
    auto& axis = connector->getData()->getAxissIni();
    QString fileName = QFileDialog::getSaveFileName(this, "Save G-code",axis.machineName(),"*.ngc");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        float pi = asin(1)*2;
        std::ofstream outFile;
        outFile.open(fileName.toLatin1().data());
        QString machineIniInfo = ";"+axis.machineName()+"\n";
        QString off1 = axis.offGcode(0) + "\n";
        QString off2 = axis.offGcode(1) + "\n";
        QString off3 = axis.offGcode(2) + "\n";
        QString off4 = axis.offGcode(3) + "\n";
        outFile<<machineIniInfo.toLatin1().data();
        outFile<<"G90 F600\n";
        outFile<<off1.toLatin1().data();
        outFile<<off2.toLatin1().data();
        outFile<<off3.toLatin1().data();
        outFile<<off4.toLatin1().data();
        outFile<<"G01\n";
        for(auto& data:moveDatas){
            QString str = axis.name(0) + " " + "[#1+" + data.origin_x(axis) + "] "
                    + axis.name(1) + " " + "[#2+" + data.origin_z(axis) + "] "
                    + axis.name(2) + " " + "[#3+" + data.origin_flip(axis) + "] "
                    + axis.name(3) + " "+ "[#4+"  + data.origin_theta(axis) +"]";
            if(data.axisSum() == 5){
                str += " " + axis.name(4) + " " + "[#5+" + data.origin_yaw(axis) + "]";
            }
            str += "\n";
            outFile<<str.toLatin1().data();
        }
        outFile<<"M30";
        outFile.close();
    }
}

void SimulationTab::calMoveData(){
    auto& axis = connector->getData()->getAxissIni();
    if(axis.axisSum() == 4){
        calAxis4Data();
    }
    else if(axis.axisSum() == 5){
        calAxis5Data();
    }
    else{
        assert(0);
    }
}

void SimulationTab::calAxis4Data(){
    auto root = connector->getData()->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));

    AxisMoveDataVec& datas = moveDatas;
    datas.clear();
    float pi = asin(1)*2;
    auto& axiss = connector->getData()->getAxissIni();
    for(auto i = 0; i < hangPtr->coupleSum(); i++){
        AxisMoveData moveData(4);
        glm::mat4 sendT = hangPtr->sendT(i);
        Pos sendPos = hangPtr->sendPos(i);
        float theta = atan2(sendPos.y, sendPos.z);
        moveData.theta(theta);
        moveData.x(sendPos.x);
        moveData.z(glm::length(glm::vec2(sendPos.z, sendPos.y)));
        glm::mat4 rotx = utility::rotx(theta);
        glm::mat4 newSendT = rotx*sendT;
        Dir newSendT_Y = newSendT[1];
        float y0 = newSendT_Y[0];
        float y1 = newSendT_Y[1];
        //张力的合力垂直于辊子
        moveData.flip(atan2(-1*y0, y1));
        if(axiss.config & AxisIni::xLeft){
            moveData.x(axiss.off(0) - moveData.x());
        }
        else{
            moveData.x(axiss.off(0) + moveData.x());
        }
        if(axiss.config & AxisIni::zDown){
            moveData.z(axiss.off(1) + moveData.z());
        }
        else{
            moveData.z(axiss.off(1) - moveData.z());
        }
        if(axiss.config & AxisIni::flipDown){
            moveData.flip(axiss.off(2) + moveData.flip());
        }
        else{
            moveData.flip(axiss.off(2) - moveData.flip());
        }
        if(axiss.config & AxisIni::spindleLeft){
            moveData.theta(axiss.off(3) - moveData.theta());
        }
        else{
            moveData.theta(axiss.off(3) + moveData.theta());
        }
        datas.push_back(moveData);
    }
    smoothData();
}

void SimulationTab::calAxis5Data(){
    auto root = connector->getData()->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));

    auto& datas = moveDatas;
    datas.clear();
    float pi = asin(1)*2;
    vector<glm::mat4> rotxs;
    auto& axiss = connector->getData()->getAxissIni();
    for(auto i = 0; i < hangPtr->coupleSum(); i++){
        AxisMoveData moveData(5);
        glm::mat4 sendT = hangPtr->sendT(i);
        Pos sendPos = hangPtr->sendPos(i);
        float theta = atan2(sendPos.y, sendPos.z);
        moveData.theta() = theta;
        moveData.x() = sendPos.x;
        moveData.z() = glm::length(glm::vec2(sendPos.z, sendPos.y));
        glm::mat4 rotx = utility::rotx(theta);
        rotxs.push_back(rotx);
        glm::mat4 newSendT = rotx*sendT;
        float x0 = newSendT[0][0];
        float x1 = newSendT[0][1];
        float yaw = atan2(x0, x1);
        moveData.yaw() = yaw;
        glm::mat4 macT = utility::roty(yaw);
        glm::vec3 Tx = macT[0];
        glm::vec3 Ty = macT[1];
        glm::vec3 y = newSendT[1];
        float flip = atan2(-1*glm::dot(Tx, y), glm::dot(Ty, y));
        moveData.flip() = flip;

        if(axiss.config & AxisIni::xLeft){
            moveData.x() = axiss.off(0) - moveData.x();
        }
        else{
            moveData.x() = axiss.off(0) + moveData.x();
        }
        if(axiss.config & AxisIni::zDown){
            moveData.z() = axiss.off(1) + moveData.z();
        }
        else{
            moveData.z() = axiss.off(1) - moveData.z();
        }
        if(axiss.config & AxisIni::spindleLeft){
            moveData.theta() = axiss.off(2) - moveData.theta();
        }
        else{
            moveData.theta() = axiss.off(2) + moveData.theta();
        }
        if(axiss.config & AxisIni::flipDown){
            moveData.flip() = axiss.off(3) + moveData.flip();
        }
        else{
            moveData.flip() = axiss.off(3) - moveData.flip();
        }
        if(axiss.config & AxisIni::yawLeft){
            moveData.yaw() = axiss.off(4) - moveData.yaw();
        }
        else{
            moveData.yaw() = axiss.off(4) + moveData.yaw();
        }
        datas.push_back(moveData);
    }
    smoothData();
}

void SimulationTab::smoothData(){
    auto& axis = connector->getData()->getAxissIni();
    float pi = asin(1)*2;
    auto& datas = moveDatas;
    float lastTheta = datas.at(0).theta();
    float lastFlip = datas.at(0).flip();
    for(auto& data:datas){
        while(data.theta() > lastTheta + pi){
            data.theta() -= 2*pi;
        }
        while(data.theta() < lastTheta - pi){
            data.theta() += 2*pi;
        }
        lastTheta = data.theta();

        while(data.flip() > lastFlip + pi){
            data.flip() -= 2*pi;
        }
        while(data.flip() < lastFlip - pi){
            data.flip() += 2*pi;
        }
        lastFlip = data.flip();
    }
}


void SimulationTab::setHeadAnimate(){
    auto root = connector->getData()->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
    auto axisIni = connector->getData()->getAxissIni();

    auto& datas = moveDatas;
    vector<glm::mat4> animateTs;
    for(auto& data:datas){
        float xpos;
        if(axisIni.config & AxisIni::xLeft){
            xpos = axisIni.off(0) - data.x();
        }
        else{
            xpos = data.x() - axisIni.off(0);
        }

        float zpos;
        if(axisIni.config & AxisIni::zDown){
            zpos = data.z() - axisIni.off(1);
        }
        else{
            zpos = axisIni.off(1) - data.z();
        }

        float theta;
        if(axisIni.config & AxisIni::spindleLeft){
            theta = axisIni.off(3) - data.theta();
        }
        else{
            theta = data.theta() - axisIni.off(3);
        }

        glm::mat4 T(1.0f);
        utility::setPos(T, Pos{xpos, 0, zpos});
        if(axisIni.axisSum() == 5){
            float yaw;
            if(axisIni.config & AxisIni::yawLeft){
                yaw = axisIni.off(4) - data.yaw();
            }
            else{
                yaw = data.yaw() - axisIni.off(4);
            }
            T = T * utility::roty(yaw);
        }

        float flip;
        if(axisIni.config & AxisIni::flipDown){
            flip = data.flip() - axisIni.off(3);
        }
        else{
            flip = axisIni.off(3) -  data.flip();
        }
        T = T * utility::rotz(flip);
        animateTs.push_back(T);
    }
    auto headPtr = root->findHeadPtr();
    headPtr->setAnimateTs(animateTs);
}

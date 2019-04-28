#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "MoveDataTab.h"
#include "GuiConnector.h"
#include "Node.h"
#include "Data.h"
#include "HangingBandSet.h"
#include "MoveData.h"
#include "AxisConfigDialog.h"
#include "QFileDialog"
#include <fstream>

MoveDataTab::MoveDataTab(TabBackground* background, GuiConnector* conn, QWidget* parent)
    :QWidget(parent), back(background), connector(conn)
{
    cancleButton = new QPushButton("close", this);
    outputButton = new QPushButton("output G-code", this);
    modifyButton = new QPushButton("modify axis", this);
    connect(modifyButton, &QPushButton::clicked, this, &MoveDataTab::modifyMachine);


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

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(machineLabel);
    layout->addWidget(axis_1_nameLabel);
    layout->addWidget(axis_2_nameLabel);
    layout->addWidget(axis_3_nameLabel);
    layout->addWidget(axis_4_nameLabel);
    layout->addWidget(axis_5_nameLabel);

    layout->addWidget(modifyButton);
    layout->addWidget(outputButton);
    layout->addWidget(cancleButton);
    layout->addStretch(1);
    setLayout(layout);
    updateLabel();

    connect(cancleButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(outputButton, &QPushButton::clicked, this, &MoveDataTab::output);
}


void MoveDataTab::calData(){
    auto& axiss = connector->getData()->getAxissIni();
    int axisSum = axiss.axisSum();
    if(axisSum == 4){
        calAxis4Data();
    }
    else if(axisSum == 5){

        calAxis5Data();
    }
    else{
        assert(0);
    }
}

void MoveDataTab::modifyMachine(){
    auto& axisIni = connector->getData()->getAxissIni();
    AxisConfigDialog* dialog = new AxisConfigDialog(&axisIni, this);
    dialog->exec();
    updateLabel();
}

void MoveDataTab::calAxis4Data(){
    auto root = connector->getData()->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));

    Axis4DataVec& datas = axis4Datas;
    datas.clear();
    float pi = asin(1)*2;
    auto& axiss = connector->getData()->getAxissIni();
    for(auto i = 0; i < hangPtr->coupleSum(); i++){
        Axis4Data moveData;
        glm::mat4 sendT = hangPtr->sendT(i);
        Pos sendPos = hangPtr->sendPos(i);
        float theta = atan2(sendPos.y, sendPos.z);
        moveData.theta = theta;
        moveData.x = sendPos.x;
        moveData.z = glm::length(glm::vec2(sendPos.z, sendPos.y));
        glm::mat4 rotx = utility::rotx(theta);
        glm::mat4 newSendT = rotx*sendT;
        Dir newSendT_Z = newSendT[2];
        float z0 = newSendT_Z[0];
        float z1 = newSendT_Z[1];
        moveData.flip = atan2(-1*z0, z1);
        if(axiss.config & AxisIni::xLeft){
            moveData.x = axiss.off(0) - moveData.x;
        }
        else{
            moveData.x = axiss.off(0) + moveData.x;
        }
        if(axiss.config & AxisIni::zDown){
            moveData.z = axiss.off(1) - moveData.z;
        }
        else{
            moveData.z = axiss.off(1) + moveData.z;
        }
        if(axiss.config & AxisIni::spindleLeft){
            moveData.theta = axiss.off(2) - moveData.theta;
        }
        else{
            moveData.theta = axiss.off(2) + moveData.theta;
        }
        if(axiss.config & AxisIni::flipDown){
            moveData.flip = axiss.off(3) + moveData.flip;
        }
        else{
            moveData.flip = axiss.off(3) - moveData.flip;
        }
        datas.push_back(moveData);
    }
    smoothData();
}

void MoveDataTab::calAxis5Data(){
    auto root = connector->getData()->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));

    Axis5DataVec& datas = axis5Datas;
    datas.clear();
    float pi = asin(1)*2;
    vector<glm::mat4> rotxs;
    auto& axiss = connector->getData()->getAxissIni();
    for(auto i = 0; i < hangPtr->coupleSum(); i++){
        Axis5Data moveData;
        glm::mat4 sendT = hangPtr->sendT(i);
        Pos sendPos = hangPtr->sendPos(i);
        float theta = atan2(sendPos.y, sendPos.z);
        moveData.theta = theta;
        moveData.x = sendPos.x;
        moveData.z = glm::length(glm::vec2(sendPos.z, sendPos.y));
        glm::mat4 rotx = utility::rotx(theta);
        rotxs.push_back(rotx);
        glm::mat4 newSendT = rotx*sendT;
        float x0 = newSendT[0][0];
        float x1 = newSendT[0][1];
        float yaw = atan2(x0, x1);
        moveData.yaw = yaw;
        glm::mat4 macT = utility::roty(yaw);
        glm::vec3 Tx = macT[0];
        glm::vec3 Ty = macT[1];
        glm::vec3 y = newSendT[1];
        float flip = atan2(-1*glm::dot(Tx, y), glm::dot(Ty, y));
        moveData.flip = flip;

        if(axiss.config & AxisIni::xLeft){
            moveData.x = axiss.off(0) - moveData.x;
        }
        else{
            moveData.x = axiss.off(0) + moveData.x;
        }
        if(axiss.config & AxisIni::zDown){
            moveData.z = axiss.off(1) - moveData.z;
        }
        else{
            moveData.z = axiss.off(1) + moveData.z;
        }
        if(axiss.config & AxisIni::spindleLeft){
            moveData.theta = axiss.off(2) - moveData.theta;
        }
        else{
            moveData.theta = axiss.off(2) + moveData.theta;
        }
        if(axiss.config & AxisIni::flipDown){
            moveData.flip = axiss.off(3) + moveData.flip;
        }
        else{
            moveData.flip = axiss.off(3) - moveData.flip;
        }
        if(axiss.config & AxisIni::yawLeft){
            moveData.yaw = axiss.off(4) + moveData.yaw;
        }
        else{
            moveData.yaw = axiss.off(4) - moveData.yaw;
        }
        datas.push_back(moveData);
    }
    smoothData();
}


void MoveDataTab::updateLabel(){
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


void MoveDataTab::output(){
    calData();
    auto& axis = connector->getData()->getAxissIni();
    QString fileName = QFileDialog::getSaveFileName(this, "Save G-code",axis.machineName(),"*.gcode");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        if(axis.axisSum() == 4){

            std::ofstream outFile;
            outFile.open(fileName.toLatin1().data());
            for(auto& data:axis4Datas){
                QString str = axis.name(0) + " " + QString::number(data.x) + " " + axis.name(1) + " " + QString::number(data.z)
                        + " " + axis.name(2) + " " + QString::number(data.theta) + " " + axis.name(3) + " " + QString::number(data.flip)
                        +"\n";
                outFile<<str.toLatin1().data();
            }
            outFile.close();
        }
        else{
            std::ofstream outFile;
            outFile.open(fileName.toLatin1().data());
            for(auto& data:axis5Datas){
                QString str = axis.name(0) + " " + QString::number(data.x) + " " + axis.name(1) + " " + QString::number(data.z)
                        + " " + axis.name(2) + " " + QString::number(data.theta) + " " + axis.name(3) + " " + QString::number(data.flip)
                        + " " + axis.name(4) + " " + QString::number(data.yaw)
                        + "\n";
                outFile<<str.toLatin1().data();
            }
            outFile.close();
        }
    }
}

void MoveDataTab::smoothData(){
    auto& axis = connector->getData()->getAxissIni();
    float pi = asin(1)*2;
    if(axis.axisSum() == 4){
        auto& datas = axis4Datas;
        float lastTheta = datas.at(0).theta;
        float lastFlip = datas.at(0).flip;
        for(auto& data:datas){
            while(data.theta > lastTheta + pi){
                data.theta -= 2*pi;
            }
            while(data.theta < lastTheta - pi){
                data.theta += 2*pi;
            }
            lastTheta = data.theta;

            while(data.flip > lastFlip + pi){
                data.flip -= 2*pi;
            }
            while(data.flip < lastFlip - pi){
                data.flip += 2*pi;
            }
            lastFlip = data.flip;
        }
    }
    else if(axis.axisSum() == 5){
        auto& datas = axis5Datas;
        float lastTheta = datas.at(0).theta;
        float lastFlip = datas.at(0).flip;
        for(auto& data:datas){
            while(data.theta > lastTheta + pi){
                data.theta -= 2*pi;
            }
            while(data.theta < lastTheta - pi){
                data.theta += 2*pi;
            }
            lastTheta = data.theta;

            while(data.flip > lastFlip + pi){
                data.flip -= 2*pi;
            }
            while(data.flip < lastFlip - pi){
                data.flip += 2*pi;
            }
            lastFlip = data.flip;
        }
    }
    else{
        assert(0);
    }
}

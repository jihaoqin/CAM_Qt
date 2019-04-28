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

MoveDataTab::MoveDataTab(TabBackground* background, GuiConnector* conn, QWidget* parent)
    :QWidget(parent), back(background), connector(conn)
{
    cancleButton = new QPushButton("close", this);
    outputButton = new QPushButton("output G-code", this);
    connect(outputButton, &QPushButton::clicked, this, &MoveDataTab::calData);
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
}


void MoveDataTab::calData(){
    auto& axiss = connector->getData()->getAxissIni();
    int axisSum = axiss.axisSum();
    if(axisSum == 4){
        calAxis4Data();
    }
    else if(axisSum == 5){

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
        datas.push_back(moveData);
    }
}

void MoveDataTab::calAxis5Data(){
    auto root = connector->getData()->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));

    Axis5DataVec& datas = axis5Datas;
    datas.clear();
    float pi = asin(1)*2;
    vector<glm::mat4> rotxs;
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
        float yProjX = newSendT[1][0];
        float yProjZ = newSendT[1][2];
        float yaw = pi/2 - atan2(yProjZ, yProjX);
        moveData.yaw = yaw;
        glm::mat4 roty = utility::roty(-1*yaw);
        glm::mat4 nnSendT = roty*newSendT;
        float xProjX = nnSendT[0][0];
        float xProjY = nnSendT[0][1];
        float flip = atan(xProjY/xProjX);
        moveData.flip = -flip;
        datas.push_back(moveData);
    }
    hangPtr->setAnimateTs(rotxs);
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

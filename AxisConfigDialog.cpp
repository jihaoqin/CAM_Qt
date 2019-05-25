#include "AxisConfigDialog.h"
#include "AxisIni.h"
#include "ui_axisconfigdialog.h"
#include <QComboBox>
#include <QFileDialog>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "ModelReader.h"
#include <fstream>
#include "GuiConnector.h"
#include "Data.h"
#include "Node.h"
#include "Model.h"
#include "Controller.h"

using namespace rapidjson;
AxisConfigDialog::AxisConfigDialog(GuiConnector* c, AxisIni* axis_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AxisConfigDialog),
    axisData(axis_), axis(new AxisIni(*axis_)), connector(c)
{
    ui->setupUi(this);
    initial();
}

AxisConfigDialog::~AxisConfigDialog()
{
    delete ui;
    delete axis;
}

void AxisConfigDialog::initial(){
    updateUI();
    QDoubleValidator *doubleReg = new QDoubleValidator(this);
    ui->hengOffLineEdit->setValidator(doubleReg);
    ui->zongOffLineEdit->setValidator(doubleReg);
    ui->spindleOffLineEdit->setValidator(doubleReg);
    ui->yawOffLineEdit->setValidator(doubleReg);
    ui->flipOffLineEdit->setValidator(doubleReg);
    connect(ui->axisSumCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAxisConfig(int)));
    connect(ui->machineLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->hengNameLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->zongNameLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->spindleNameLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->flipNameLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->yawNameLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->hengOffLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->zongOffLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->spindleOffLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->flipOffLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->yawOffLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);


    connect(ui->spindleLeftCheckbox, &QCheckBox::clicked, ui->spindleRightCheckbox, &QCheckBox::click);
    connect(ui->spindleRightCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::spindleLeftCheckbox);
    connect(ui->xLeftCheckbox, &QCheckBox::clicked, ui->xRightCheckbox, &QCheckBox::click);
    connect(ui->xRightCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::xLeftCheckbox);
    connect(ui->zDownCheckbox, &QCheckBox::clicked, ui->zUpCheckBox, &QCheckBox::click);
    connect(ui->zUpCheckBox, &QCheckBox::stateChanged, this, &AxisConfigDialog::zDownCheckbox);
    connect(ui->flipDownCheckbox, &QCheckBox::clicked, ui->flipUpCheckbox, &QCheckBox::click);
    connect(ui->flipUpCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::flipDownCheckbox);
    connect(ui->yawLeftCheckbox, &QCheckBox::clicked, ui->yawRightCheckbox, &QCheckBox::click);
    connect(ui->yawRightCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::yawLeftCheckbox);

    connect(ui->cancleButton, &QPushButton::clicked, this, &AxisConfigDialog::close);
    connect(ui->okButton, &QPushButton::clicked, this, &AxisConfigDialog::saveIni);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &AxisConfigDialog::saveInFile);
    connect(ui->importButton, &QPushButton::clicked, this, &AxisConfigDialog::importFromFile);

    connect(ui->loadModelButton, &QPushButton::clicked, this, &AxisConfigDialog::loadModel);
}

void AxisConfigDialog::updateUI(){
    ui->axisSumCombox->blockSignals(true);
    ui->machineLineEdit->blockSignals(true);
    ui->hengNameLineEdit->blockSignals(true);
    ui->zongNameLineEdit->blockSignals(true);
    ui->spindleNameLineEdit->blockSignals(true);
    ui->flipNameLineEdit->blockSignals(true);
    ui->yawNameLineEdit->blockSignals(true);
    ui->hengOffLineEdit->blockSignals(true);
    ui->zongOffLineEdit->blockSignals(true);
    ui->spindleOffLineEdit->blockSignals(true);
    ui->flipOffLineEdit->blockSignals(true);
    ui->yawOffLineEdit->blockSignals(true);

    ui->machineLineEdit->setText(axis->machineName());
    ui->headRLine->setText(QString::number(axis->getHeadR()));
    int axisSum = axis->axisSum();
    if(axisSum == 4){
        ui->axisSumCombox->setCurrentIndex(0);
        ui->yawNameLineEdit->hide();
        ui->yawOffLineEdit->hide();
        ui->yawLeftCheckbox->hide();
        ui->yawRightCheckbox->hide();
    }
    else if(axisSum == 5){
        ui->axisSumCombox->setCurrentIndex(1);
        ui->yawNameLineEdit->show();
        ui->yawOffLineEdit->show();
        ui->yawLeftCheckbox->show();
        ui->yawRightCheckbox->show();
    }
    else{
        assert(0);
    }

    {
        if(axis->config & AxisIni::spindleLeft){
            ui->spindleLeftCheckbox->setChecked(true);
            ui->spindleRightCheckbox->setChecked(false);
        }
        else{
            ui->spindleLeftCheckbox->setChecked(false);
            ui->spindleRightCheckbox->setChecked(true);
        }

        if(axis->config & AxisIni::xLeft){
            ui->xLeftCheckbox->setChecked(true);
            ui->xRightCheckbox->setChecked(false);
        }
        else{
            ui->xLeftCheckbox->setChecked(false);
            ui->xRightCheckbox->setChecked(true);
        }

        if(axis->config & AxisIni::zDown){
            ui->zDownCheckbox->setChecked(true);
            ui->zUpCheckBox->setChecked(false);
        }
        else{
            ui->zDownCheckbox->setChecked(false);
            ui->zUpCheckBox->setChecked(true);
        }

        {
            if(axis->config & AxisIni::flipDown){
                ui->flipDownCheckbox->setChecked(true);
                ui->flipUpCheckbox->setChecked(false);
            }
            else{
                ui->flipDownCheckbox->setChecked(false);
                ui->flipUpCheckbox->setChecked(true);
            }
        }

        {
            if(axis->axisSum() == 5){
                if(axis->config & AxisIni::yawLeft){
                    ui->yawLeftCheckbox->setChecked(true);
                    ui->yawRightCheckbox->setChecked(false);
                }
                else{
                    ui->yawLeftCheckbox->setChecked(false);
                    ui->yawRightCheckbox->setChecked(true);
                }
            }
        }
    }

    {
        float pi = asin(1)*2;
        ui->hengNameLineEdit->setText(axis->name(0));
        ui->hengOffLineEdit->setText(axis->offStr(0));

        ui->zongNameLineEdit->setText(axis->name(1));
        ui->zongOffLineEdit->setText(axis->offStr(1));

        ui->flipNameLineEdit->setText(axis->name(2));
        ui->flipOffLineEdit->setText(QString::number(axis->off(2)*180/pi));

        ui->spindleNameLineEdit->setText(axis->name(3));
        ui->spindleOffLineEdit->setText(QString::number(axis->off(3)*180/pi));

        if(axisSum == 5){
            ui->yawNameLineEdit->setText(axis->name(4));
            ui->yawOffLineEdit->setText(QString::number(axis->off(4)*180/pi));
        }
    }

    ui->axisSumCombox->blockSignals(false);
    ui->machineLineEdit->blockSignals(false);
    ui->hengNameLineEdit->blockSignals(false);
    ui->zongNameLineEdit->blockSignals(false);
    ui->spindleNameLineEdit->blockSignals(false);
    ui->flipNameLineEdit->blockSignals(false);
    ui->yawNameLineEdit->blockSignals(false);
    ui->hengOffLineEdit->blockSignals(false);
    ui->zongOffLineEdit->blockSignals(false);
    ui->spindleOffLineEdit->blockSignals(false);
    ui->flipOffLineEdit->blockSignals(false);
    ui->yawOffLineEdit->blockSignals(false);
}


void AxisConfigDialog::spindleLeftCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->spindleLeftCheckbox->setChecked(false);
    }
    else{
        ui->spindleLeftCheckbox->setChecked(true);
        axis->config |= AxisIni::spindleLeft;
    }
}

void AxisConfigDialog::xLeftCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->xLeftCheckbox->setChecked(false);
    }
    else{
        ui->xLeftCheckbox->setChecked(true);
        axis->config |= AxisIni::xLeft;
    }
}

void AxisConfigDialog::zDownCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->zDownCheckbox->setChecked(false);
    }
    else{
        ui->zDownCheckbox->setChecked(true);
        axis->config |= AxisIni::zDown;
    }
}

void AxisConfigDialog::flipDownCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->flipDownCheckbox->setChecked(false);
    }
    else{
        ui->flipDownCheckbox->setChecked(true);
        axis->config |= AxisIni::flipDown;
    }
}

void AxisConfigDialog::yawLeftCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->yawLeftCheckbox->setChecked(false);
    }
    else{
        ui->yawLeftCheckbox->setChecked(true);
        axis->config |= AxisIni::yawLeft;
    }
}


void AxisConfigDialog::setAxisConfig(int ind){
    if(ind == 0){
        axis->setAxisSum(4);
    }
    else if(ind == 1){
        axis->setAxisSum(5);
    }
    else{
        assert(0);
    }
    updateUI();
}


void AxisConfigDialog::saveIni(){
    *axisData = *axis;
    close();
}

void AxisConfigDialog::updateAxis(){
    axis->machineName(ui->machineLineEdit->text());
    if(ui->axisSumCombox->currentIndex() == 0){
        axis->setAxisSum(4);
    }
    else{
        axis->setAxisSum(5);
    }
    axis->setHeadR(ui->headRLine->text().toFloat());
    axis->setName(0, ui->hengNameLineEdit->text());
    axis->setName(1,ui->zongNameLineEdit->text());
    axis->setName(2,ui->flipNameLineEdit->text());
    axis->setName(3,ui->spindleNameLineEdit->text());

    float pi = asin(1)*2;
    axis->setOff(0, ui->hengOffLineEdit->text().toDouble());
    axis->setOff(1, ui->zongOffLineEdit->text().toDouble());
    axis->setOff(2, ui->flipOffLineEdit->text().toDouble()*pi/180);
    axis->setOff(3, ui->spindleOffLineEdit->text().toDouble()*pi/180);

    if(axis->axisSum() == 5){
        axis->setName(4,ui->yawNameLineEdit->text());
        axis->setOff(4, ui->yawOffLineEdit->text().toDouble()*pi/180);
    }

    if(ui->xLeftCheckbox->checkState() == Qt::CheckState::Checked){
        axis->config |= AxisIni::xLeft;
    }
    if(ui->zDownCheckbox->checkState() == Qt::Checked){
        axis->config |= AxisIni::zDown;
    }
    if(ui->flipDownCheckbox->checkState() == Qt::Checked){
        axis->config |= AxisIni::flipDown;
    }
    if(ui->spindleLeftCheckbox->checkState() == Qt::Checked){
        axis->config |= AxisIni::spindleLeft;
    }


    if(axis->axisSum() == 5){
        if(ui->yawLeftCheckbox->checkState() == Qt::Checked){
            axis->config |= AxisIni::yawLeft;
        }
    }
}

void AxisConfigDialog::saveInFile(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save AxisIni",axis->machineName(),"*.axis");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        axis->serialize(writer);
        std::ofstream outFile;
        outFile.open(fileName.toLatin1().data());
        outFile<<sb.GetString();
        outFile.close();
    }
}

void AxisConfigDialog::importFromFile(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open AxisIni","","*.axis");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        std::ifstream inFile;
        std::string line;
        std::string allLine;
        inFile.open(fileName.toLatin1().data());
        while(getline(inFile, line)){
            allLine.append(line+"\n");
        }
        Document doc;
        doc.Parse<0>(allLine.c_str());
        if(doc.HasParseError()){
            return ;
        }
        assert(doc["type"] == "AxisIni");
        QString machine = doc["machine"].GetString();
        int axisSum = doc["axisSum"].GetInt();
        float r = doc["head radius"].GetDouble();
        std::vector<float> axis_Offs;
        const Value& offs = doc["axis_Offs"];
        for(int i =0; i<offs.Size(); i++){
            axis_Offs.push_back(offs[i].GetDouble());
        }
        QStringVec axis_Names;
        const Value& names = doc["names"];
        for(int i =0; i<names.Size(); i++){
            axis_Names.push_back(names[i].GetString());
        }
        int config = doc["config"].GetInt();
        delete axis;
        axis = new AxisIni(axisSum);
        axis->machineName(machine);
        axis->setAxisSum(axisSum);
        axis->setHeadR(r);
        for(int i = 0; i<axisSum; i++){
            axis->setOff(i, axis_Offs.at(i));
            axis->setName(i, axis_Names.at(i));
        }
        axis->config = config;
        updateUI();
    }
}


void AxisConfigDialog::loadModel(){
    QString fileName = QFileDialog::getOpenFileName(this, "load model","","*.STL");
    if(fileName.isEmpty()){
        return ;
    }
    else{
        ModelReader loader(fileName.toLatin1().data());
        auto& mesh = loader.modelMesh();
        auto headPtr = connector->getData()->getNodeRoot()->findHeadPtr();
        headPtr->setMeshData(mesh.vertexs(), mesh.indexs());
        connector->getCtrl()->allBindGL();
    }
}

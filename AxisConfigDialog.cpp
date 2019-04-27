#include "AxisConfigDialog.h"
#include "AxisIni.h"
#include "ui_axisconfigdialog.h"
#include <QComboBox>

AxisConfigDialog::AxisConfigDialog(AxisIni* axis_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AxisConfigDialog),
    axisData(axis_), axis(new AxisIni(*axis_))
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
    connect(ui->hengNameLineEdit, &QLineEdit::textChanged, this, &AxisConfigDialog::updateAxis);
    connect(ui->cancleButton, &QPushButton::clicked, this, &AxisConfigDialog::close);
    connect(ui->okButton, &QPushButton::clicked, this, &AxisConfigDialog::saveIni);
}

void AxisConfigDialog::updateUI(){
    {
        ui->machineLineEdit->setText(axis->machineName());
    }
    int axisSum = axis->axisSum();
    if(axisSum == 4){
        ui->axisSumCombox->setCurrentIndex(0);
        ui->yawNameLineEdit->setEnabled(false);
        ui->yawOffLineEdit->setEnabled(false);
    }
    else if(axisSum == 5){
        ui->axisSumCombox->setCurrentIndex(1);
        ui->yawNameLineEdit->setEnabled(true);
        ui->yawOffLineEdit->setEnabled(true);
        ui->yawLeftCheckbox->setEnabled(true);
        ui->yawRightCheckbox->setEnabled(true);
    }
    else{
        ui->axisSumCombox->setCurrentIndex(0);
    }

    {
        ui->machineLineEdit->setText(axis->machineName());
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
        connect(ui->spindleLeftCheckbox, &QCheckBox::clicked, ui->spindleRightCheckbox, &QCheckBox::click);
        connect(ui->spindleRightCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::spindleLeftCheckbox);

        if(axis->config & AxisIni::xLeft){
            ui->xLeftCheckbox->setChecked(true);
            ui->xRightCheckbox->setChecked(false);
        }
        else{
            ui->xLeftCheckbox->setChecked(false);
            ui->xRightCheckbox->setChecked(true);
        }
        connect(ui->xLeftCheckbox, &QCheckBox::clicked, ui->xRightCheckbox, &QCheckBox::click);
        connect(ui->xRightCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::xLeftCheckbox);

        if(axis->config & AxisIni::zDown){
            ui->zDownCheckbox->setChecked(true);
            ui->zUpCheckBox->setChecked(false);
        }
        else{
            ui->zDownCheckbox->setChecked(false);
            ui->zUpCheckBox->setChecked(true);
        }
        connect(ui->zDownCheckbox, &QCheckBox::clicked, ui->zUpCheckBox, &QCheckBox::click);
        connect(ui->zUpCheckBox, &QCheckBox::stateChanged, this, &AxisConfigDialog::zDownCheckbox);

        {
            if(axis->config & AxisIni::flipDown){
                ui->flipDownCheckbox->setChecked(true);
                ui->flipUpCheckbox->setChecked(false);
            }
            else{
                ui->flipDownCheckbox->setChecked(false);
                ui->flipUpCheckbox->setChecked(true);
            }
            connect(ui->flipDownCheckbox, &QCheckBox::clicked, ui->flipUpCheckbox, &QCheckBox::click);
            connect(ui->flipUpCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::flipDownCheckbox);
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
                connect(ui->yawLeftCheckbox, &QCheckBox::clicked, ui->yawRightCheckbox, &QCheckBox::click);
                connect(ui->yawRightCheckbox, &QCheckBox::stateChanged, this, &AxisConfigDialog::yawLeftCheckbox);
            }
            else{
                ui->yawLeftCheckbox->setEnabled(false);
                ui->yawRightCheckbox->setEnabled(false);
            }
        }
    }

    {
        ui->hengNameLineEdit->setText(axis->name(0));
        ui->hengOffLineEdit->setText(axis->offStr(0));

        ui->zongNameLineEdit->setText(axis->name(1));
        ui->zongOffLineEdit->setText(axis->offStr(1));

        ui->spindleNameLineEdit->setText(axis->name(2));
        ui->spindleOffLineEdit->setText(axis->offStr(2));

        ui->flipNameLineEdit->setText(axis->name(3));
        ui->flipOffLineEdit->setText(axis->offStr(3));

        ui->yawNameLineEdit->setText(axis->name(4));
        ui->yawOffLineEdit->setText(axis->offStr(4));
    }
}


void AxisConfigDialog::spindleLeftCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->spindleLeftCheckbox->setChecked(false);
    }
    else{
        ui->spindleLeftCheckbox->setChecked(true);
    }
}

void AxisConfigDialog::xLeftCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->xLeftCheckbox->setChecked(false);
    }
    else{
        ui->xLeftCheckbox->setChecked(true);
    }
}

void AxisConfigDialog::zDownCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->zDownCheckbox->setChecked(false);
    }
    else{
        ui->zDownCheckbox->setChecked(true);
    }
}

void AxisConfigDialog::flipDownCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->flipDownCheckbox->setChecked(false);
    }
    else{
        ui->flipDownCheckbox->setChecked(true);
    }
}

void AxisConfigDialog::yawLeftCheckbox(int state){
    if(state == Qt::CheckState::Checked){
        ui->yawLeftCheckbox->setChecked(false);
    }
    else{
        ui->yawLeftCheckbox->setChecked(true);
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

}

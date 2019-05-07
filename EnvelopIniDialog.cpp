#include "EnvelopIniDialog.h"
#include "ui_envelopinidialog.h"
#include "GuiConnector.h"
#include "Data.h"
#include "Node.h"
#include <QDoubleValidator>

EnvelopIniDialog::EnvelopIniDialog(GuiConnector* c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnvelopIniDialog),
    connector(c)
{
    ui->setupUi(this);
    auto tee = connector->getData()->getNodeRoot()->findTeePtr();
    auto envelop = connector->getData()->getEnvelopeIni();
    ui->DiameterLabel->setText(QString::number(tee->teePara().pipeR*2));
    ui->mainLengthLabel->setText(QString::number(tee->teePara().lengthMain));
    ui->branchLenghtLabel->setText(QString::number(tee->teePara().lengthBranch));
    ui->incRLineEdit->setText(QString::number(envelop.incPipeR));
    ui->incBLLineEdit->setText(QString::number(envelop.incBLength));
    ui->incMLLineEdit->setText(QString::number(envelop.incMLength));

    QDoubleValidator *doubleReg = new QDoubleValidator(this);
    ui->incMLLineEdit->setValidator(doubleReg);
    ui->incBLLineEdit->setValidator(doubleReg);
    ui->incRLineEdit->setValidator(doubleReg);

    connect(ui->okButton, &QPushButton::clicked, this, &EnvelopIniDialog::okPressed);
    connect(ui->cancleButton, &QPushButton::clicked, this, &EnvelopIniDialog::close);
}

EnvelopIniDialog::~EnvelopIniDialog()
{
    delete ui;
}


void EnvelopIniDialog::okPressed(){
    tempIni.incMLength = ui->incMLLineEdit->text().toFloat();
    tempIni.incBLength = ui->incBLLineEdit->text().toFloat();
    tempIni.incPipeR = ui->incRLineEdit->text().toFloat();
    connector->getData()->getEnvelopeIni() = tempIni;
    close();
}


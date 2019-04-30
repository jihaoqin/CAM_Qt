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
    ui->incLLineEdit->setText(QString::number(envelop.incLength));

    QDoubleValidator *doubleReg = new QDoubleValidator(this);
    ui->incLLineEdit->setValidator(doubleReg);
    ui->incRLineEdit->setValidator(doubleReg);

    connect(ui->incLLineEdit, &QLineEdit::textChanged, this, &EnvelopIniDialog::updateTempIni);
    connect(ui->incRLineEdit, &QLineEdit::textChanged, this, &EnvelopIniDialog::updateTempIni);
    connect(ui->okButton, &QPushButton::clicked, this, &EnvelopIniDialog::okPressed);
    connect(ui->cancleButton, &QPushButton::clicked, this, &EnvelopIniDialog::close);
}

EnvelopIniDialog::~EnvelopIniDialog()
{
    delete ui;
}


void EnvelopIniDialog::updateTempIni(){
    tempIni.incLength = ui->incLLineEdit->text().toFloat();
    tempIni.incPipeR = ui->incRLineEdit->text().toFloat();
}

void EnvelopIniDialog::okPressed(){
    connector->getData()->getEnvelopeIni() = tempIni;
    close();
}


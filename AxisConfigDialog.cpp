#include "AxisConfigDialog.h"
#include "ui_axisconfigdialog.h"

AxisConfigDialog::AxisConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AxisConfigDialog)
{
    ui->setupUi(this);
}

AxisConfigDialog::~AxisConfigDialog()
{
    delete ui;
}

void AxisConfigDialog::initial(){
}

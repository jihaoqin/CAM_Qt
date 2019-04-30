#include "TeeParameterDialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMessageBox>

TeeParameterDialog::TeeParameterDialog(MainWindow *parent):QDialog(dynamic_cast<QWidget*>(parent))
{
    setWindowTitle("Tee Parameter Dialog");
    //setFixedSize(400, 400);
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("cancel", this);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    const int lineWidth = 50;
    const int labelWidth = 120;
    QDoubleValidator *doubleReg = new QDoubleValidator(this);
    doubleReg->setBottom(0.1);

    mainLengthLine = new QLineEdit(this);
    mainLengthLine->setValidator(doubleReg);
    mainLengthLine->setFixedWidth(lineWidth);
    mainLengthLabel = new QLabel(this);
    mainLengthLabel->setText("Main length");
    mainLengthLabel->setFixedWidth(labelWidth);
    QHBoxLayout *hLayout_1 = new QHBoxLayout;
    hLayout_1->addWidget(mainLengthLabel);
    hLayout_1->addWidget(mainLengthLine);


    branchLengthLine = new QLineEdit(this);
    branchLengthLine->setValidator(doubleReg);
    branchLengthLine->setFixedWidth(lineWidth);
    branchLengthLabel = new QLabel(this);
    branchLengthLabel->setText("Branch length");
    branchLengthLabel->setFixedWidth(labelWidth);
    QHBoxLayout *hLayout_2 = new QHBoxLayout;
    hLayout_2->addWidget(branchLengthLabel);
    hLayout_2->addWidget(branchLengthLine);

    sideRLine = new QLineEdit(this);
    sideRLine->setValidator(doubleReg);
    sideRLine->setFixedWidth(lineWidth);
    sideRLabel = new QLabel(this);
    sideRLabel->setFixedWidth(labelWidth);
    sideRLabel->setText("Side R");
    QHBoxLayout *hLayout_3 = new QHBoxLayout;
    hLayout_3->addWidget(sideRLabel);
    hLayout_3->addWidget(sideRLine);

    RLine = new QLineEdit(this);
    RLine->setValidator(doubleReg);
    RLine->setFixedWidth(lineWidth);
    RLabel = new QLabel(this);
    RLabel->setFixedWidth(labelWidth);
    RLabel->setText("R");
    QHBoxLayout *hLayout_4 = new QHBoxLayout;
    hLayout_4->addWidget(RLabel);
    hLayout_4->addWidget(RLine);

    widthLine = new QLineEdit(this);
    widthLine->setValidator(doubleReg);
    widthLine->setFixedWidth(lineWidth);
    widthLabel = new QLabel("width", this);
    widthLine->setFixedWidth(lineWidth);
    QHBoxLayout *hLayout_5 = new QHBoxLayout;
    hLayout_5->addWidget(widthLabel);
    hLayout_5->addWidget(widthLine);

    teePic = new QLabel(this);
    QPixmap teePixmap(":/images/tee");
    teePic->setPixmap(teePixmap);
    QHBoxLayout* rightLayout = new QHBoxLayout;
    rightLayout->addWidget(teePic);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addLayout(hLayout_1);
    leftLayout->addLayout(hLayout_2);
    leftLayout->addLayout(hLayout_3);
    leftLayout->addLayout(hLayout_4);
    leftLayout->addLayout(hLayout_5);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addLayout(leftLayout);
    hLayout->addLayout(rightLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(hLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);

    connect(okButton, &QPushButton::pressed, this, &TeeParameterDialog::readData);
    connect(cancelButton, &QPushButton::pressed, this, &QDialog::close);
}


void TeeParameterDialog::readData(){
    float sideR = sideRLine->text().toFloat();
    float R = RLine->text().toFloat();
    float mainLength = mainLengthLine->text().toFloat();
    float branchLength = branchLengthLine->text().toFloat();
    if(!isValid(mainLength, branchLength, R, sideR)){
        QMessageBox::information(NULL, "Info", "Invalid data!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else{
        para = TeePara{R, sideR, mainLength, branchLength};
        m_width = widthLine->text().toFloat();
        close();
        setResult(QDialog::Accepted);
    }
}

bool TeeParameterDialog::isValid(float mainLength, float branchLength, float R , float sideR){
    if(mainLength <= 0){
        return false;
    }
    if(branchLength <= 0){
        return false;
    }
    if(R<=0){
        return false;
    }
    if(sideR<=0){
        return false;
    }
    if(mainLength < 2*(sideR+R)){
        return false;
    }
    else{
        if(branchLength < (2*R+sideR)){
            return false;
        }
        else{
            return true;
        }
    }
}

TeePara TeeParameterDialog::getTeePara(){
    return para;
}


float TeeParameterDialog::bandWidth(){
    return m_width;
}

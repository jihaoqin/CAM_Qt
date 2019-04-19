#include "NewCurveTab.h"
#include <QDebug>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include "GuiConnector.h"
#include <QEvent>
#include <QColor>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "NewCurveController.h"
#include "TabBackground.h"
NewCurveTab::NewCurveTab(QWidget* parent):QWidget(parent),focusOnPoint(true),curveId("")
{
    pointBox = new QGroupBox("Point",this);
    pointLabel = new QLabel("point",this);
    pointText = new QTextEdit(this);
    pointText->setFixedHeight(50);
    pointText->setReadOnly(true);
    pointText->setStyleSheet(QString(":focus{ background-color: #E6E6E6; }"));
    QFormLayout* layout_1 = new QFormLayout(pointBox);
    layout_1->addWidget(pointLabel);
    layout_1->addWidget(pointText);
    pointBox->setLayout(layout_1);

    dirBox = new QGroupBox("Direction",this);
    dirLabel = new QLabel("winding angle:",this);
    dirSlider = new QSlider(this);
    dirSlider->setOrientation(Qt::Horizontal);
    dirSlider->setRange(-90, 90);
    dirSpinBox = new QSpinBox(this);
    dirSpinBox->setRange(-90, 90);
    dirSpinBox->setSingleStep(1);
    dirSpinBox->setValue(45);
    lamLabel = new QLabel("slippery", this);
    lamSpinBox = new QDoubleSpinBox(this);
    lamSpinBox->setDecimals(3);
    lamSpinBox->setRange(-0.1, 0.1);
    lamSpinBox->setSingleStep(0.005);
    lamSpinBox->setValue(0);
    connect(dirSpinBox, SIGNAL(valueChanged(int)), dirSlider, SLOT(setValue(int)));
    connect(dirSlider, SIGNAL(valueChanged(int)), dirSpinBox, SLOT(setValue(int)));
    connect(dirSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCurve()));
    connect(lamSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateCurve()));
    QVBoxLayout* layout_2 = new QVBoxLayout();
    QFormLayout* layout_22 = new QFormLayout();
    QFormLayout* layout_21 = new QFormLayout();
    layout_21->addWidget(dirLabel);
    layout_21->addWidget(dirSpinBox);
    layout_22->addWidget(lamLabel);
    layout_22->addWidget(lamSpinBox);
    layout_2->addLayout(layout_21);
    layout_2->addWidget(dirSlider);
    layout_2->addLayout(layout_22);
    dirBox->setLayout(layout_2);

    ok = new QPushButton("Ok", this);
    ok->setEnabled(false);
    cancle = new QPushButton("Cancle",this);
    connect(ok, &QPushButton::clicked, this, &NewCurveTab::okPressed);
    connect(cancle, &QPushButton::clicked, this, &NewCurveTab::canclePressed);
    QHBoxLayout* layout_3 = new QHBoxLayout();
    layout_3->addWidget(ok);
    layout_3->addWidget(cancle);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pointBox);
    mainLayout->addWidget(dirBox);
    mainLayout->addLayout(layout_3);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    pointText->viewport()->installEventFilter(this);
    //pointText->installEventFilter(this);
}


NewCurveTab::~NewCurveTab(){
}


void NewCurveTab::setConnector(GuiConnector* c){
    connector = c;
}

bool NewCurveTab::eventFilter(QObject *target, QEvent *event){
    if(target == pointText->viewport()){
        if(event->type() == QEvent::MouseButtonPress){
            connector->setGLWidgetClickable(true);
            focusOnPoint = true;
            return true;
        }
        else if(event->type() == QEvent::Enter){
            pointText->viewport()->setCursor(Qt::ArrowCursor);
            return true;
        }
        else{
            return QWidget::eventFilter(target, event);
        }
    }
    else{
        return QWidget::eventFilter(target, event);
    }
}

QString NewCurveTab::getPointText(){
    QString pointId = pointText->toPlainText();
    return pointId;
}

void NewCurveTab::setPointText(QString id){
    pointText->setText(id);
    ok->setEnabled(true);
}

void NewCurveTab::setCurveId(QString name){
    curveId = name;
}

bool NewCurveTab::isPointTextFocused(){
    return focusOnPoint;
}

float NewCurveTab::getWindingAngle(){
    float pi = asin(1)*2;
    return dirSpinBox->value()*pi/180;
}

void NewCurveTab::updateCurve(){
    if(curveId.isEmpty()){
        return;
    }
    float angle = dirSpinBox->value();
    float coe = lamSpinBox->value();
    connector->updateCurve(curveId, angle*1.0/180*3.1415926, coe);
}

void NewCurveTab::okPressed(){
    NewCurveController* c = new NewCurveController(this, connector->getCtrl(), getPointText(), curveId);
    connector->setGLWidgetClickable(false);
    c->okPressed();
    back->setWidget(nullptr);
    connector->updateAction();
    close();
}
void NewCurveTab::canclePressed(){
    NewCurveController* c = new NewCurveController(this, connector->getCtrl(), getPointText(), curveId);
    connector->setGLWidgetClickable(false);
    c->canclePressed();
    back->setWidget(nullptr);
    connector->updateAction();
    close();
}

void NewCurveTab::setTabBack(TabBackground *t){
    assert(t);
    back = t;
}

float NewCurveTab::getSlip(){
    return lamSpinBox->value();
}

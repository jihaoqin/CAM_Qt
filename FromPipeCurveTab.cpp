#include "FromPipeCurveTab.h"
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QFormLayout>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QEvent>
#include "GuiConnector.h"
#include "FromPipeCurveController.h"
#include "TabBackground.h"
#include <QCheckBox>

FromPipeCurveTab::FromPipeCurveTab(TabBackground* background, GuiConnector* conn, QWidget* parent)
    :QWidget(parent), connector(conn), back(background)
{
    initial();
}


void FromPipeCurveTab::initial(){
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
    oppositeBox = new QCheckBox("symmetry", this);
    oppositeBox->setCheckState(Qt::Unchecked);
    dirSlider = new QSlider(this);
    dirSlider->setOrientation(Qt::Horizontal);
    dirSlider->setRange(2, 30);
    dirSlider->setValue(10);
    dirSpinBox = new QSpinBox(this);
    dirSpinBox->setRange(2, 30);
    dirSpinBox->setSingleStep(1);
    dirSpinBox->setValue(10);
    lamLabel = new QLabel("slippery", this);
    lamSpinBox = new QDoubleSpinBox(this);
    lamSpinBox->setDecimals(3);
    lamSpinBox->setRange(-0.1, 0.1);
    lamSpinBox->setSingleStep(0.005);
    lamSpinBox->setValue(0);
    connect(dirSpinBox, SIGNAL(valueChanged(int)), dirSlider, SLOT(setValue(int)));
    connect(dirSlider, SIGNAL(valueChanged(int)), dirSpinBox, SLOT(setValue(int)));
    connect(dirSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateBand()));
    connect(lamSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateBand()));
    connect(oppositeBox, SIGNAL(stateChanged(int)), this, SLOT(updateBand()));
    QVBoxLayout* layout_2 = new QVBoxLayout();
    QFormLayout* layout_22 = new QFormLayout();
    QFormLayout* layout_21 = new QFormLayout();
    layout_21->addWidget(dirLabel);
    layout_21->addWidget(dirSpinBox);
    layout_21->addWidget(oppositeBox);
    layout_22->addWidget(lamLabel);
    layout_22->addWidget(lamSpinBox);
    layout_2->addLayout(layout_21);
    layout_2->addWidget(dirSlider);
    layout_2->addLayout(layout_22);
    dirBox->setLayout(layout_2);

    ok = new QPushButton("Ok", this);
    ok->setEnabled(false);
    cancle = new QPushButton("Cancle",this);
    connect(ok, &QPushButton::clicked, this, &FromPipeCurveTab::okPressed);
    connect(cancle, &QPushButton::clicked, this, &FromPipeCurveTab::canclePressed);
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
}


void FromPipeCurveTab::okPressed(){
    FromPipeCurveController* c = new FromPipeCurveController(this, connector->getCtrl());
    connector->setGLWidgetClickable(false);
    c->okPressed();
    back->setWidget(nullptr);
    connector->updateModel();
    connector->updateAction();
    close();
}

void FromPipeCurveTab::canclePressed(){
    FromPipeCurveController* c = new FromPipeCurveController(this, connector->getCtrl());
    connector->setGLWidgetClickable(false);
    c->canclePressed();
    back->setWidget(nullptr);
    connector->updateModel();
    connector->updateAction();
    close();
}


bool FromPipeCurveTab::eventFilter(QObject* target, QEvent* event){
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


QString FromPipeCurveTab::getPointText(){
    QString pointId = pointText->toPlainText();
    return pointId;
}

bool FromPipeCurveTab::isPointTextFocused(){
    return focusOnPoint;
}

void FromPipeCurveTab::setPointText(QString id){
    pointText->setText(id);
    ok->setEnabled(true);
}

float FromPipeCurveTab::getWindingAngle(){
    float pi = asin(1)*2;
    int symbol = oppositeBox->isChecked()? -1:1;
    return dirSpinBox->value()*pi/180*symbol;
}


void FromPipeCurveTab::setCBandId(QString id){
    cBandId = id;
}

void FromPipeCurveTab::updateBand(){
    if(cBandId.isEmpty()){
        return;
    }
    FromPipeCurveController* ctrl = new FromPipeCurveController(this, connector->getCtrl());
    QString cPointId = getPointText();
    ctrl->updateBandUsing(cPointId, cBandId, tPointId, tCurveId);
}


QString FromPipeCurveTab::getCBandId(){
    return cBandId;
}


void FromPipeCurveTab::setTPointId(QString t){
    tPointId = t;
}

void FromPipeCurveTab::setTCurveId(QString tband){
    tCurveId = tband;
}


float FromPipeCurveTab::getSlip(){
    return lamSpinBox->value();
}


QString FromPipeCurveTab::getTPointId(){
    return tPointId;
}


QString FromPipeCurveTab::getTCurveId(){
    return tCurveId;
}

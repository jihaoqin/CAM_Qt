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
}

void FromPipeCurveTab::canclePressed(){

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

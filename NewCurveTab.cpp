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

NewCurveTab::NewCurveTab(QWidget* parent):QWidget(parent),focusOnPoint(true)
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
    dirSpinBox = new QSpinBox(this);
    //dirSpinBox->setFixedSize(QSize(10,10));
    dirSpinBox->setRange(1,89);
    dirSpinBox->setSingleStep(1);
    connect(dirSpinBox, SIGNAL(valueChanged(int)), dirSlider, SLOT(setValue(int)));
    connect(dirSlider, SIGNAL(valueChanged(int)), dirSpinBox, SLOT(setValue(int)));
    dirSpinBox->setValue(45);
    QVBoxLayout* layout_21 = new QVBoxLayout();
    QFormLayout* layout_2 = new QFormLayout();
    layout_2->addWidget(dirLabel);
    layout_2->addWidget(dirSpinBox);
    layout_21->addLayout(layout_2);
    layout_21->addWidget(dirSlider);
    dirBox->setLayout(layout_21);

    ok = new QPushButton("Ok", this);
    cancle = new QPushButton("Cancle",this);
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



bool NewCurveTab::isPointTextFocused(){
    return focusOnPoint;
}

int NewCurveTab::getWindingAngle(){
    return dirSpinBox->value();
}

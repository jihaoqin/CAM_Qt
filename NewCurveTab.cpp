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

NewCurveTab::NewCurveTab(QWidget* parent):QWidget(parent)
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
    dirLabel = new QLabel("direction",this);
    dirText = new QTextEdit(this);
    dirText->setFixedHeight(50);
    dirText->setReadOnly(true);
    dirText->setStyleSheet(QString(":focus{ background-color: #E6E6E6; }"));
    QFormLayout* layout_2 = new QFormLayout(dirBox);
    layout_2->addWidget(dirLabel);
    layout_2->addWidget(dirText);
    dirBox->setLayout(layout_2);

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
}


NewCurveTab::~NewCurveTab(){
}


void NewCurveTab::setConnector(GuiConnector* c){
    connector = c;
}

bool NewCurveTab::eventFilter(QObject *target, QEvent *event){
    qDebug()<<target;
    if(target == pointText->viewport()){
        if(event->type() == QEvent::MouseButtonPress){
            //connector->setGLWidgetMouseClickable(true);
            return true;
        }
        else if(event->type() == QEvent::Enter){
            pointText->viewport()->setCursor(Qt::ArrowCursor);
            return true;
        }
        else if(event->type() == QEvent::FocusOut){
            //connector->setGLWidgetMouseClickable(false);
            return true;
        }
    }
    else{
        return QWidget::eventFilter(target, event);
    }
}

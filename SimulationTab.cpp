#include "SimulationTab.h"
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "AnimateController.h"
#include "GuiConnector.h"
#include "TabBackground.h"
#include <QTimer>

SimulationTab::SimulationTab(TabBackground* back, GuiConnector* con, QWidget* parent)
    :QWidget(parent), backWidget(back), connector(con)
{
    calButton = new QPushButton("calculation", this);
    playAndPauseButton = new QPushButton(QIcon(":/icons/play"), "", this);
    playAndPauseButton ->setEnabled(false);
    slowButton = new QPushButton(QIcon(":/icons/sf"),"", this);
    slowButton->setEnabled(false);
    closeButton = new QPushButton("close", this);
    closeButton->setEnabled(true);
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setEnabled(false);
    progressSlider->setRange(0,100);
    fastButton = new QPushButton(QIcon(":/icons/ff"),"", this);
    fastButton->setEnabled(false);
    fFrameButton = new QPushButton(QIcon(":/icons/fFrame"),"", this);
    fFrameButton->setEnabled(false);
    bFrameButton = new QPushButton(QIcon(":/icons/bFrame"), "", this);
    bFrameButton->setEnabled(false);
    animateCtrl = new AnimateController(connector->getCtrl());
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(slowButton);
    hLayout->addWidget(bFrameButton);
    hLayout->addWidget(playAndPauseButton);
    hLayout->addWidget(fFrameButton);
    hLayout->addWidget(fastButton);
    layout->addWidget(calButton);
    layout->addLayout(hLayout);
    layout->addWidget(progressSlider);
    layout->addWidget(closeButton);
    layout->addStretch(1);
    setLayout(layout);
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(calButton, &QPushButton::clicked, this, &SimulationTab::calculation);
    connect(playAndPauseButton, &QPushButton::clicked, this, &SimulationTab::playOrPause);
    connect(slowButton, &QPushButton::clicked, this, &SimulationTab::slowForward);
    connect(closeButton, &QPushButton::clicked, this, &SimulationTab::closeMyself);
    connect(fastButton, &QPushButton::clicked, this, &SimulationTab::fastForward);
    connect(progressSlider, &QSlider::valueChanged, this, &SimulationTab::setPercent);
    connect(fFrameButton, &QPushButton::clicked, this, &SimulationTab::nextFrame);
    connect(bFrameButton, &QPushButton::clicked, this, &SimulationTab::lastFrame);
    connect(timer, &QTimer::timeout, this, &SimulationTab::showNext);
}

void SimulationTab::calculation(){
    animateCtrl->calculation();
    if(!animateCtrl->hasData()){
        return;
    }
    playAndPauseButton->setEnabled(true);
    slowButton->setEnabled(true);
    progressSlider->setEnabled(true);
    fastButton->setEnabled(true);
    fFrameButton->setEnabled(true);
    bFrameButton->setEnabled(true);
}

void SimulationTab::playOrPause(){
    if(timer->isActive()){
        timer->stop();
        playAndPauseButton->setIcon(QIcon(":/icons/play"));
    }
    else{
        timer->start();
        playAndPauseButton->setIcon(QIcon(":/icons/pause"));
    }
}

void SimulationTab::showNext(){
    animateCtrl->showNext();
    int percent = animateCtrl->getPercent();
    progressSlider->blockSignals(true);
    progressSlider->setValue(percent);
    progressSlider->blockSignals(false);
}

void SimulationTab::closeMyself(){
    timer->stop();
    animateCtrl->resetBand();
    animateCtrl->hideBandSet();
    backWidget->setWidget(nullptr);
    close();
}

void SimulationTab::setPercent(int percent){
    animateCtrl->setPercent(percent);
}

void SimulationTab::fastForward(){
    int  intl = timer->interval();
    int nextIntl = intl/2;
    if(nextIntl<=0){
        nextIntl = 1;
    }
    timer->setInterval(nextIntl);
}


void SimulationTab::pause(){
    timer->stop();
    playAndPauseButton->setIcon(QIcon(":/icons/play"));
}


void SimulationTab::slowForward(){
    int  intl = timer->interval();
    int nextIntl = intl*2;
    timer->setInterval(nextIntl);
}

void SimulationTab::nextFrame(){
    showNext();
}

void SimulationTab::lastFrame(){

}

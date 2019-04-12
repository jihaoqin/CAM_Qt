#include "SimulationTab.h"
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include "AnimateController.h"
#include "GuiConnector.h"
#include "TabBackground.h"
#include <QTimer>

SimulationTab::SimulationTab(TabBackground* back, GuiConnector* con, QWidget* parent)
    :QWidget(parent), backWidget(back), connector(con)
{
    calButton = new QPushButton("calculation", this);
    goonButton = new QPushButton("play",this);
    goonButton->setEnabled(false);
    pauseButton = new QPushButton("pause", this);
    pauseButton->setEnabled(false);
    closeButton = new QPushButton("close", this);
    closeButton->setEnabled(true);
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setEnabled(false);
    progressSlider->setRange(0,100);
    fastButton = new QPushButton("ff", this);
    fastButton->setEnabled(false);
    animateCtrl = new AnimateController(connector->getCtrl());
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(calButton);
    layout->addWidget(goonButton);
    layout->addWidget(pauseButton);
    layout->addWidget(fastButton);
    layout->addWidget(progressSlider);
    layout->addWidget(closeButton);
    layout->addStretch(1);
    setLayout(layout);
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(calButton, &QPushButton::clicked, this, &SimulationTab::calculation);
    connect(goonButton, &QPushButton::clicked, this, &SimulationTab::goon);
    connect(pauseButton, &QPushButton::clicked, timer, &QTimer::stop);
    connect(closeButton, &QPushButton::clicked, this, &SimulationTab::closeMyself);
    connect(fastButton, &QPushButton::clicked, this, &SimulationTab::fastForward);
    connect(progressSlider, &QSlider::valueChanged, this, &SimulationTab::setPercent);
    connect(timer, &QTimer::timeout, this, &SimulationTab::showNext);
}

void SimulationTab::calculation(){
    animateCtrl->calculation();
    goonButton->setEnabled(true);
    pauseButton->setEnabled(true);
    progressSlider->setEnabled(true);
    fastButton->setEnabled(true);
}

void SimulationTab::goon(){
    timer->start();
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

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
    goonButton = new QPushButton("continue",this);
    goonButton->setEnabled(false);
    pauseButton = new QPushButton("pause", this);
    pauseButton->setEnabled(false);
    closeButton = new QPushButton("close", this);
    closeButton->setEnabled(true);
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setEnabled(false);
    progressSlider->setRange(0,100);
    animateCtrl = new AnimateController(connector->getCtrl());
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(calButton);
    layout->addWidget(goonButton);
    layout->addWidget(pauseButton);
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
    connect(timer, &QTimer::timeout, this, &SimulationTab::showNext);
}

void SimulationTab::calculation(){
    animateCtrl->calculation();
    goonButton->setEnabled(true);
    pauseButton->setEnabled(true);
    progressSlider->setEnabled(true);
}

void SimulationTab::goon(){
    timer->start();
}

void SimulationTab::showNext(){
    animateCtrl->showNext();
    int percent = animateCtrl->getPercent();
    progressSlider->setValue(percent);
}

void SimulationTab::closeMyself(){
    timer->stop();
    animateCtrl->resetBand();
    backWidget->setWidget(nullptr);
    close();
}

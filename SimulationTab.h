#ifndef SIMULATIONTAB_H
#define SIMULATIONTAB_H
#include <QWidget>
#include "NameDeclare.h"

class QPushButton;
class QSlider;
class QTimer;
class SimulationTab:public QWidget
{
public:
    SimulationTab(TabBackground*, GuiConnector*, QWidget* parent = nullptr);
public slots:
    void calculation();
    void playOrPause();
    void showNext();
    void closeMyself();
    void setPercent(int);
    void fastForward();
    void slowForward();
    void pause();
    void nextFrame();
    void lastFrame();
private:
    QPushButton* calButton;
    QPushButton* playAndPauseButton;
    QPushButton* closeButton;
    QPushButton* slowButton;
    QPushButton* fastButton;
    QPushButton* fFrameButton;
    QPushButton* bFrameButton;
    QSlider* progressSlider;
    QTimer* timer;

    GuiConnector* connector;
    TabBackground* backWidget;
    AnimateController* animateCtrl;
};

#endif // SIMULATIONTAB_H

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
    void goon();
    void showNext();
    void closeMyself();
    void setPercent(int);
    void fastForward();
    void pause();
private:
    QPushButton* calButton;
    QPushButton* goonButton;
    QPushButton* pauseButton;
    QPushButton* closeButton;
    QPushButton* fastButton;
    QSlider* progressSlider;
    QTimer* timer;

    GuiConnector* connector;
    TabBackground* backWidget;
    AnimateController* animateCtrl;
};

#endif // SIMULATIONTAB_H

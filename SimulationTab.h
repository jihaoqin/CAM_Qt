#ifndef SIMULATIONTAB_H
#define SIMULATIONTAB_H
#include <QWidget>
#include "NameDeclare.h"
#include "MoveData.h"

class QPushButton;
class QSlider;
class QTimer;
class QCheckBox;
class QLabel;
class SimulationTab:public QWidget
{
public:
    SimulationTab(TabBackground*, GuiConnector*, QWidget* parent = nullptr);
    bool shallShowHangLines();
    bool isAbsolute();
    void updateLabel();
    void smoothData();
    void setHeadAnimate();
public slots:
    void output();
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
    void showEnvelopIniDialog();
    void modifyMachine();
private:
    void calMoveData();
    void calAxis4Data();
    void calAxis5Data();

    QLabel* machineLabel;
    QLabel* axis_1_nameLabel;
    QLabel* axis_2_nameLabel;
    QLabel* axis_3_nameLabel;
    QLabel* axis_4_nameLabel;
    QLabel* axis_5_nameLabel;

    QPushButton* outputButton;
    QPushButton* axisIniButton;
    QPushButton* envelopButton;
    QPushButton* calButton;
    QCheckBox* absoluteBox;
    QCheckBox* hangVisableBox;
    QPushButton* playAndPauseButton;
    QPushButton* closeButton;
    QPushButton* slowButton;
    QPushButton* fastButton;
    QPushButton* fFrameButton;
    QPushButton* bFrameButton;
    QSlider* progressSlider;
    QTimer* timer;

    AxisMoveDataVec moveDatas;

    GuiConnector* connector;
    TabBackground* backWidget;
    AnimateController* animateCtrl;
};

#endif // SIMULATIONTAB_H

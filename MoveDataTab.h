#ifndef MOVEDATATAB_H
#define MOVEDATATAB_H
#include <QWidget>
#include <vector>
#include "NameDeclare.h"
#include "MoveData.h"

class QPushButton;
class QLabel;

class MoveDataTab:public QWidget
{
    Q_OBJECT
public:
    MoveDataTab(TabBackground*, GuiConnector*, QWidget* parent = nullptr);
    void updateLabel();
public slots:
    void calData();
    void calAxis4Data();
    void calAxis5Data();
    void modifyMachine();

private:
    QPushButton* cancleButton;
    QPushButton* outputButton;
    QPushButton* modifyButton;

    QLabel* machineLabel;
    QLabel* axis_1_nameLabel;
    QLabel* axis_2_nameLabel;
    QLabel* axis_3_nameLabel;
    QLabel* axis_4_nameLabel;
    QLabel* axis_5_nameLabel;

    Axis4DataVec axis4Datas;
    Axis5DataVec axis5Datas;

    GuiConnector* connector;
    TabBackground* back;
};

#endif // MOVEDATATAB_H

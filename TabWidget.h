#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QMouseEvent>

class TabBackground;
class QLabel;
class GuiConnector;
class ObjTreeWidget;

class TabWidget: public QTabWidget
{
    Q_OBJECT
    friend class GuiConnector;
public:
    TabWidget(QWidget* parent = nullptr);
    bool isOperating();
    void setConnector(GuiConnector*);
public slots:
    void showNewCurveTab();
    void showSimulationTab();
private:
    ObjTreeWidget* tree;
    TabBackground* operation;
    GuiConnector* connector;
};


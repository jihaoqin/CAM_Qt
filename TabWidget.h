#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QMouseEvent>

class TabBackground;
class QLabel;
class GuiConnector;

class TabWidget: public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget* parent = nullptr);
    bool isOperating();
    void setConnector(GuiConnector*);
    friend class GuiConnector;
public slots:
    void showNewCurveTab();
private:
    TabBackground* first;
    TabBackground* operation;
    GuiConnector* connector;
};


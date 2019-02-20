#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QMouseEvent>

class TabBackground;
class QLabel;
class TabWidget: public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget* parent = nullptr);
    bool isOperating();
public slots:
    void showNewCurveTab();
private:
    TabBackground* first;
    TabBackground* operation;
};


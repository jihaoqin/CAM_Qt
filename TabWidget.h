#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QMouseEvent>

class TabBackground;
class NewCurveTab;
class QLabel;
class TabWidget: public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget* parent = nullptr);
public slots:
    void setOpTab(QWidget* w);
private:
    TabBackground* first;
    TabBackground* operation;
};


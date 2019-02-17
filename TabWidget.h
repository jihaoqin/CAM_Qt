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
private:
    TabBackground* first;
    TabBackground* second;
};


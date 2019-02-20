#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "TabWidget.h"
#include "glwidget.h"

class StrechLabel;

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    void bindController(Controller*);
public slots:
    void showNewCurveTab();
    void doSome();
protected:
    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
private slots:
    void strech(int deltaX);
private:
    TabWidget* tabWidget;
    GLWidget* glWidget;
    StrechLabel* block;
    Controller* ctrl;
};

#endif // CENTRALWIDGET_H

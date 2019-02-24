#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "TabWidget.h"
#include "glwidget.h"

class StrechLabel;
class GuiConnector;

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    void bindController(Controller*);
    bool isOperating();
    void setConnector(GuiConnector*);
    friend class GuiConnector;
public slots:
    void showNewCurveTab();
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
    GuiConnector* connector;
};

#endif // CENTRALWIDGET_H

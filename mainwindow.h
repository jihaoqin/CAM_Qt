#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "Model.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void bindController(Controller*);
    ~MainWindow();
private:
    GLWidget widget;
};

#endif // MAINWINDOW_H

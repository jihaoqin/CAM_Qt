#pragma once
#include <QMainWindow>
#include "glwidget.h"
#include "Model.h"
class TeeParameterDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void bindController(Controller*);
    ~MainWindow();
public slots:
    void showTeeParameterDialog();
    void saveOrNot();
private:
    void configureStatusBar();
    void configureMenuBar();
    Controller* ctrl;
    GLWidget widget;
    TeeParameterDialog *teeNewDialog;
};


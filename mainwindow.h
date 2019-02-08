#pragma once
#include <QMainWindow>
#include "glwidget.h"
#include "Model.h"
#include <QMenuBar>
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
    void updateMenu();
private:
    void configureStatusBar();
    void configureMenuBar();
    Controller* ctrl;
    GLWidget widget;
    TeeParameterDialog *teeNewDialog;
    QMenu* fileMenu;
};


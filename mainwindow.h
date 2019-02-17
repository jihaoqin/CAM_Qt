#pragma once
#include <QMainWindow>
#include <QMenuBar>
class TeeParameterDialog;
class CentralWidget;
class Controller;
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
    void updateAction();
private:
    void configureStatusBar();
    void configureMenuBar();
    void configureToolBar();
    Controller* ctrl;
    CentralWidget* widget;
    TeeParameterDialog *teeNewDialog;
    QMenu* fileMenu;
    QToolBar* toolBar;
};


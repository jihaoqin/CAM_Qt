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
    MainWindow( Controller* c, QWidget *parent = 0);
    void bindController(Controller*);
    ~MainWindow();
public slots:
    void showTeeParameterDialog();
    void saveOrNot();
    void updateAction();
private slots:
    void showNewCurveTab();
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


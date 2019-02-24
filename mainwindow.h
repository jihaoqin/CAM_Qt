#pragma once
#include <QMainWindow>
#include <QMenuBar>
class TeeParameterDialog;
class CentralWidget;
class Controller;
class GuiConnector;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( Controller* c, QWidget *parent = 0);
    void bindController(Controller*);
    ~MainWindow();
    friend class GuiConnector;
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
    GuiConnector* connector;
    QMenu* fileMenu;
    QToolBar* toolBar;
};


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
    GuiConnector* connector;
public slots:
    void showTeeParameterDialog();
    void saveOrNot();
    void updateAction();
private slots:
    void showNewCurveTab();
    void saveBand();
    void openBand();
private:
    void configureStatusBar();
    void configureMenuBar();
    void configureToolBar();
    Controller* ctrl;
    CentralWidget* widget;
    TeeParameterDialog *teeNewDialog;
    QMenu* fileMenu;
        QAction* actionNew;
        QAction* actionOpen;
        QAction* actionSave;
    QMenu* bandMenu;
        QAction* bandSave;
        QAction* bandOpen;
    //ToolBar
        QAction *newCurve;
        QAction *leftCurve;
        QAction *rightCurve;
        QAction *upCurve;
    QToolBar* toolBar;
};


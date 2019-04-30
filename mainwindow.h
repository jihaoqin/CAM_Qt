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
    void allBind();
    void updateModel();
    void loopBand();
    void animationOver();
    void showSimTab();
private slots:
    void showNewCurveTab();
    void showFromPipeCurveTab();
    void saveBand();
    void openBand();
    void genLeftCurve();
    void genUpCurve();
    void genRightCurve();
    void saveIni();
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
        QAction *fromPipeCurve;
        QAction *leftCurve;
        QAction *rightCurve;
        QAction *upCurve;
        QAction *loopCurve;
        QAction *simAction;
    QToolBar* toolBar;
};


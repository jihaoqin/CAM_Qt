#ifndef GUICONNECTOR_H
#define GUICONNECTOR_H
#include <QString>
#include <memory>
class MainWindow;
class GLProgram;
class NewCurveTab;
class GLWidget;
class Controller;
class Data;
class GenCurveProgressDialog;
class TabWidget;
class SimulationTab;
class FromPipeCurveTab;

class GuiConnector
{
public:
    GuiConnector();
    void setRoot(MainWindow*);
    void setGLWidgetClickable(bool);
    void updateCurve(QString, float, float);
    void updateAction();
    GenCurveProgressDialog* getGenCurveProgressDlg();
    NewCurveTab* getNewCurveTabWidget();
    FromPipeCurveTab* getFromPipeCurveTab();
    std::shared_ptr<GLProgram> getMeshProgram();
    std::shared_ptr<GLProgram> getPointProgram();
    QString getPointText();
    void setPointText(QString);
    GLWidget* getGLWidget();
    Controller* getCtrl();
    std::shared_ptr<Data> getData();
    TabWidget* getTabWidget();
    void updateModel();
    SimulationTab* getSimTab();
private:
    MainWindow* mainWindow;
};

#endif // GUICONNECTOR_H

#ifndef GUICONNECTOR_H
#define GUICONNECTOR_H
#include <QString>
#include <memory>
class MainWindow;
class GLProgram;
class NewCurveTab;
class GLWidget;

class GuiConnector
{
public:
    GuiConnector();
    void setRoot(MainWindow*);
    void setGLWidgetClickable(bool);
    void updateCurve(QString, float);
    NewCurveTab* getNewCurveTabWidget();
    std::shared_ptr<GLProgram> getMeshProgram();
    std::shared_ptr<GLProgram> getPointProgram();
    QString getPointText();
    void setPointText(QString);
    GLWidget* getGLWidget();

private:
    MainWindow* mainWindow;
};

#endif // GUICONNECTOR_H

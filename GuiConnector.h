#ifndef GUICONNECTOR_H
#define GUICONNECTOR_H
#include <QString>

class MainWindow;

class GuiConnector
{
public:
    GuiConnector();
    void setRoot(MainWindow*);
    void setGLWidgetClickable(bool);
    QString getPointText();
private:
    MainWindow* mainWindow;
};

#endif // GUICONNECTOR_H

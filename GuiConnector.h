#ifndef GUICONNECTOR_H
#define GUICONNECTOR_H

class MainWindow;

class GuiConnector
{
public:
    GuiConnector();
    void setRoot(MainWindow*);
    void setGLWidgetClickable(bool);
    bool isPointTextEmpty();
private:
    MainWindow* mainWindow;
};

#endif // GUICONNECTOR_H

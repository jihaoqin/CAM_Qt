#ifndef NEWCURVECONTROLLER_H
#define NEWCURVECONTROLLER_H
#include <QString>
#include <memory>
class Controller;
class Data;
class NewCurveTab;
class GuiConnector;
class Point;

using PointPtr =std::shared_ptr<Point>;

class NewCurveController
{
public:
    NewCurveController(NewCurveTab*, Controller*, QString pointId, QString curveId);
    void okPressed();
    void canclePressed();
    void hide(QString id);
private:
    void addSymmetry(PointPtr p, QString flag);
    NewCurveTab* tab;
    GuiConnector* connector;
    std::shared_ptr<Data> data;
    QString pointId;
    QString curveId;
};

#endif // NEWCURVECONTROLLER_H

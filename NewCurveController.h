#ifndef NEWCURVECONTROLLER_H
#define NEWCURVECONTROLLER_H
#include <QString>

class Controller;
class Data;

class NewCurveController
{
public:
    NewCurveController(Controller*, QString pointId, QString curveId);
    void okPressed();
    void canclePressed();
    void hide(QString id);
private:
    std::shared_ptr<Data> data;
    QString pointId;
    QString curveId;
};

#endif // NEWCURVECONTROLLER_H

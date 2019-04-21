#ifndef FROMPIPECURVECONTROLLER_H
#define FROMPIPECURVECONTROLLER_H
#include <QString>
#include "Data.h"

class Controller;
class FromPipeCurveTab;

class FromPipeCurveController
{
public:
    FromPipeCurveController(FromPipeCurveTab*, Controller*);
    void updateBandUsing(QString cPointId, QString cBandId, QString tPointId, QString tBandId);
    void okPressed();
    void canclePressed();
    void addSymmetry(PointPtr p, QString flag);
    void hide(QString);
private:
    FromPipeCurveTab* tab;
    Controller* controller;
    std::shared_ptr<Data> data;
};

#endif // FROMPIPECURVECONTROLLER_H

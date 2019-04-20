#ifndef FROMPIPECURVECONTROLLER_H
#define FROMPIPECURVECONTROLLER_H
#include <QString>
#include "Data.h"

class Controller;
class FromPipeCurveTab;
class GuiConnector;

class FromPipeCurveController
{
public:
    FromPipeCurveController(FromPipeCurveTab*, Controller*);
    void updateBandUsing(QString cPointId, QString cBandId, QString tPointId, QString tBandId);
private:
    FromPipeCurveTab* tab;
    //GuiConnector* connector;
    std::shared_ptr<Data> data;
};

#endif // FROMPIPECURVECONTROLLER_H

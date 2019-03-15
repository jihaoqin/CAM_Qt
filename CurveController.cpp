#include "CurveController.h"
#include "Controller.h"
#include "Data.h"
CurveController::CurveController(Controller* c):ctrl(c)
{
    assert(ctrl);
}

void CurveController::updateCurve(QString id, float angle){
}

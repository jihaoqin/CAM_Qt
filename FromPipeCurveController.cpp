#include "FromPipeCurveController.h"
#include "Node.h"
#include "CBandOnPoint.h"
#include "FromPipeCurveTab.h"
#include "Point.h"
#include "Controller.h"

FromPipeCurveController::FromPipeCurveController(FromPipeCurveTab* t, Controller* c)
    :tab(t), data(c->getData())
{

}

void FromPipeCurveController::updateBandUsing(QString pointId, QString bandId){
    PointPtr pointPtr = data->getNodeRoot()->findPointPtr(pointId);
    BandPtr bandPtr = data->getNodeRoot()->findBandPtr(bandId);
    CBandPtr cBand = std::dynamic_pointer_cast<CBandOnPoint>(bandPtr);
    cBand->setPara(HalfPoint{pointPtr->getPos(),  pointPtr->meshId()}, tab->getWindingAngle());
}

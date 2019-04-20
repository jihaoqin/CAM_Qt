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

void FromPipeCurveController::updateBandUsing(QString cPointId, QString cBandId, QString tPointId , QString tBandId){
    PointPtr cPointPtr = data->getNodeRoot()->findPointPtr(cPointId);
    BandPtr cBandPtr = data->getNodeRoot()->findBandPtr(cBandId);
    CBandPtr cBand = std::dynamic_pointer_cast<CBandOnPoint>(cBandPtr);
    cBand->setPara(HalfPoint{cPointPtr->getPos(),  cPointPtr->meshId()}, tab->getWindingAngle());

    PointPtr tPointPtr = data->getNodeRoot()->findPointPtr(tPointId);
    tPointPtr->setPos(cBand->bandEnd()->frontPos());
    TeePtr tee = data->getNodeRoot()->findTeePtr();
    tPointPtr->meshId(tee->cBand2TBandMesh(cBand->frontMeshId()).toLatin1().data());
}

#include "FromPipeCurveController.h"
#include "Node.h"
#include "CBandOnPoint.h"
#include "FromPipeCurveTab.h"
#include "Point.h"
#include "Controller.h"
#include "RingAssist.h"
#include "CylinderAssist.h"

FromPipeCurveController::FromPipeCurveController(FromPipeCurveTab* t, Controller* c)
    :tab(t), data(c->getData())
{

}

void FromPipeCurveController::updateBandUsing(QString cPointId, QString cBandId, QString tPointId , QString tCurveId){
    PointPtr cPointPtr = data->getNodeRoot()->findPointPtr(cPointId);
    BandPtr cBandPtr = data->getNodeRoot()->findBandPtr(cBandId);
    CBandPtr cBand = std::dynamic_pointer_cast<CBandOnPoint>(cBandPtr);
    cBand->setPara(HalfPoint{cPointPtr->getPos(),  cPointPtr->meshId()}, tab->getWindingAngle());

    PointPtr tPointPtr = data->getNodeRoot()->findPointPtr(tPointId);
    tPointPtr->setPos(cBand->bandEnd()->frontPos());
    TeePtr tee = data->getNodeRoot()->findTeePtr();
    tPointPtr->meshId(tee->cBand2TBandMesh(cBand->frontMeshId()).toLatin1().data());

    CurvePtr tCurvePtr = data->getNodeRoot()->findCurvePtr(tCurveId);
    Dir tCurveDir = cBand->bandEnd()->frontEnd()->pd.dir;
    QString tPointMeshId = tPointPtr->meshId();
    if(tPointMeshId.contains("ring")){
        Ring* ring  = tee->getRing(tPointMeshId);
        RingAssist rAssist(*ring);
        CPPara para = rAssist.worldToCPPara(tPointPtr->getPos(), tCurveDir);
        tCurvePtr->setWindingPara(para.uAng, tab->getSlip());
    }
    else if(tPointMeshId.contains("cylinder")){
        Cylinder* cylinder  = tee->getCylinder(tPointMeshId);
        CylinderAssist assist(*cylinder);
        CPPara para = assist.worldToCPPara(tPointPtr->getPos(), tCurveDir);
        tCurvePtr->setWindingPara(para.uAng, tab->getSlip());
    }
    else{
        assert(0);
    }
}

#include "FromPipeCurveController.h"
#include "Node.h"
#include "CBandOnPoint.h"
#include "FromPipeCurveTab.h"
#include "Point.h"
#include "Controller.h"
#include "RingAssist.h"
#include "CylinderAssist.h"
#include "PointSym.h"
#include "TCurve.h"
#include "TBandOnPoint.h"
#include "GuiConnector.h"
#include "glwidget.h"

FromPipeCurveController::FromPipeCurveController(FromPipeCurveTab* t, Controller* c)
    :tab(t), controller(c), data(c->getData())
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


void FromPipeCurveController::okPressed(){
    PointPtr tPointPtr = data->getNodeRoot()->findPointPtr(tab->getTPointId());
    auto childIds = tPointPtr->childId();
    QString tBandId;
    for(auto c:childIds){
        if(c.contains("band")){
            tBandId = c;
            break;
        }
    }
    BandPtr tBand = data->getNodeRoot()->findBandPtr(tBandId);
    tBand->setColor(Color::YELLOW);
    addSymmetry(tPointPtr, "x");
    addSymmetry(tPointPtr, "z");
    addSymmetry(tPointPtr, "xz");
    data->getNodeRoot()->deleteChild(tab->getPointText().toLatin1().data());
    data->getNodeRoot()->deleteChild(tab->getCBandId().toLatin1().data());
    hide(tab->getTPointId());
    hide(tab->getTCurveId());
}

void FromPipeCurveController::canclePressed(){
    if(tab->getPointText().isEmpty()){
        return ;
    }
    data->getNodeRoot()->deleteChild(tab->getPointText().toLatin1().data());
    data->getNodeRoot()->deleteChild(tab->getCBandId().toLatin1().data());
    PointPtr tPointPtr = data->getNodeRoot()->findPointPtr(tab->getTPointId());
    for(auto id:tPointPtr->childId()){
        data->getNodeRoot()->deleteChild(id.toLatin1().data());
    }
    data->getNodeRoot()->deleteChild(tab->getTPointId().toLatin1().data());
}

void FromPipeCurveController::addSymmetry(PointPtr p, QString flag){
    int i = pow(-1, flag.size());
    float pi = asin(1)*2;
    TeePtr tee = data->getNodeRoot()->findTeePtr();
    auto childIds = p->childId();
    QString tCurveId;
    for(auto c: childIds){
        if(c.contains("curve")){
            tCurveId = c;
            break;
        }
    }
    TCurvePtr tCurvePtr = std::dynamic_pointer_cast<TCurve>(data->getNodeRoot()->findCurvePtr(tCurveId.toLatin1().data()));
    float windAngle = tCurvePtr->windAngle();
    float lam = tab->getSlip();
    QString pxId = data->idGenerator.getPointId();
    PointSymPtr px = std::make_shared<PointSym>(p, pxId,flag, tee);
    QString curvexId = data->idGenerator.getCurveId();
    TCurvePtr curvex = std::make_shared<TCurve>(px,windAngle*i,lam*i, curvexId.toLatin1().data(),tee);
    QString bandxId = data->idGenerator.getBandId();
    TBandOnPointPtr bandx = std::make_shared<TBandOnPoint>(px, data->bandWidth(), curvex, bandxId, tee);
    bandx->setColor(Color::YELLOW);
    p->addChild(px);
    px->addChild(curvex);
    curvex->addChild(bandx);
    data->addPoint(px);
    data->addCurve(curvex);
    data->addBand(bandx);
    GuiConnector* connector = controller->getConnector();
    connector->updateModel();
    auto gl = connector->getGLWidget();
    auto context = gl->getGLContext();
    px->bindGL(context);
    curvex->bindGL(context);
    bandx->bindGL(context);
    px->setVisiable(false);
    curvex->setVisiable(false);
}


void FromPipeCurveController::hide(QString id){
    DataObjectPtr node = data->root->findObjectId(id.toLatin1().data());
    if(nullptr != node){
        node->setVisiable(false);
    }
}

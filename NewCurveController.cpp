#include "NewCurveController.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"
#include "Band.h"
#include "TBandOnPoint.h"
#include "Color.h"
#include <memory>
#include "PointSym.h"
#include "NewCurveTab.h"
#include "GuiConnector.h"
#include "glwidget.h"
using namespace std;

NewCurveController::NewCurveController(NewCurveTab* n, Controller* c, QString pointId_, QString curveId_)
    :data(c->data), pointId(pointId_), curveId(curveId_), tab(n), connector(n->connector)
{

}

void NewCurveController::okPressed(){
    hide(pointId);
    hide(curveId);
    DataObjectPtr node = data->root->findObjectId(pointId.toLatin1().data());
    PointPtr p = dynamic_pointer_cast<Point>(node);
    for(auto& id:node->childId()){
        if(id.contains("band")){
            DataObjectPtr b = data->root->findObjectId(id.toLatin1().data());
            BandPtr band = std::dynamic_pointer_cast<Band>(b);
            band->setColor(Color::YELLOW);
        }
        else{        }
    }
    addSymmetry(p, "x");
    addSymmetry(p, "z");
    addSymmetry(p, "xz");
}

void NewCurveController::hide(QString id){
    DataObjectPtr node = data->root->findObjectId(id.toLatin1().data());
    if(nullptr != node){
        node->setVisiable(false);
    }

}

void NewCurveController::canclePressed(){
    DataObjectPtr node = data->root->findObjectId(pointId.toLatin1().data());
    vector<QString> child = node->childId();
    for(auto& s:child){
        data->root->deleteChild(s.toLatin1().data());
    }
    data->root->deleteChild(pointId.toLatin1().data());
}

void NewCurveController::addSymmetry(PointPtr p, QString flag){
    int i = pow(-1, flag.size());
    float pi = asin(1)*2;
    auto teeBase = data->root->findObjectId("tee");
    TeePtr tee = dynamic_pointer_cast<Tee>(teeBase);
    float windAngle = tab->getWindingAngle()*pi/180;
    float lam = tab->getSlip();
    QString pxId = data->idGenerator.getPointId();
    PointSymPtr px = std::make_shared<PointSym>(p, pxId,flag, tee);
    QString curvexId = data->idGenerator.getCurveId();
    TCurvePtr curvex = std::make_shared<TCurve>(px,windAngle*i,lam*i, curvexId.toLatin1().data(),tee);
    QString bandxId = data->idGenerator.getBandId();
    TBandOnPointPtr bandx = std::make_shared<TBandOnPoint>(px, 2, curvex, bandxId, tee);
    bandx->setColor(Color::YELLOW);
    px->addChild(curvex);
    px->addChild(bandx);
    curvex->addChild(bandx);
    data->addPoint(px);
    data->addCurve(curvex);
    data->addBand(bandx);
    auto gl = connector->getGLWidget();
    auto context = gl->getGLContext();
    px->bindGL(context);
    curvex->bindGL(context);
    bandx->bindGL(context);
    px->setVisiable(false);
    curvex->setVisiable(false);
}

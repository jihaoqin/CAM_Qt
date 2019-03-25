#include "NewCurveController.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"
#include "Band.h"
#include "TBandOnPoint.h"
#include "Color.h"
#include <memory>

NewCurveController::NewCurveController(Controller* c, QString pointId_, QString curveId_)
    :data(c->data), pointId(pointId_), curveId(curveId_)
{

}

void NewCurveController::okPressed(){
    hide(pointId);
    hide(curveId);
    DataObjectPtr node = data->root->findObjectId(pointId.toLatin1().data());
    for(auto& id:node->childId()){
        if(id.contains("band")){
            DataObjectPtr b = data->root->findObjectId(id.toLatin1().data());
            BandPtr band = std::dynamic_pointer_cast<Band>(b);
            band->setColor(Color::YELLOW);
        }
        else{

        }
    }
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

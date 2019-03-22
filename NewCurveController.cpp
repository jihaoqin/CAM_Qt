#include "NewCurveController.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"

NewCurveController::NewCurveController(Controller* c, QString pointId_, QString curveId_)
    :data(c->data), pointId(pointId_), curveId(curveId_)
{

}

void NewCurveController::okPressed(){
    hide(pointId);
}

void NewCurveController::hide(QString id){
    DataObjectPtr node = data->root->findObjectId(id.toLatin1().data());
    if(nullptr != node){
        node->setVisiable(false);
    }

}

void NewCurveController::canclePressed(){
    //do nothing
}

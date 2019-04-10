#include "AnimateController.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"

AnimateController::AnimateController(Controller* c)
    :ctrl(c)
{

}

void AnimateController::initBandPtrs(){
    DataPtr data = ctrl->getData();
    NodePtr root = data->getNodeRoot();
    auto children = root->childrenPtrVec();
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr bandPtr = std::dynamic_pointer_cast<Band>(c);
            bandPtrs.push_back(bandPtr);
        }
    }
}

void AnimateController::initIndexPairVecs(){
    BandPtr bandPtr = bandPtrs.at(0);
    auto bandEnd = bandPtr->bandEnd();
    if(bandEnd->frontEnd()->nextEndId.isEmpty()){
        bandPtr->getGLIndexPairVec(bandEnd->frontEnd(), m_distance);
    }
}

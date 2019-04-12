#include "AnimateController.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"
#include "mainwindow.h"


AnimateController::AnimateController(Controller* c)
    :ctrl(c)
{
    m_distance = 10;
    bandInd = 0;
    showInd = 0;
    pairTotal = 0;
    windedTotal = 0;
}

void AnimateController::initBandPtrs(){
    DataPtr data = ctrl->getData();
    NodePtr root = data->getNodeRoot();
    auto children = root->childrenPtrVec();
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr bandPtr = std::dynamic_pointer_cast<Band>(c->getData());
            bandPtrs.push_back(bandPtr);
        }
    }
}

void AnimateController::initIndexPairVecs(){
    if(bandPtrs.size() == 0){
        return;
    }
    BandPtr bandPtr = bandPtrs.at(0);
    auto bandEnd = bandPtr->bandEnd();
    Pos lastPos;
    if(bandEnd->frontEnd()->nextEndId.isEmpty()){
        lastPos = bandEnd->frontEnd()->pd.pos;
    }
    else{
        lastPos = bandEnd->backEnd()->pd.pos;
    }
    for(auto b:bandPtrs){
        Pos fEndPos = b->bandEnd()->frontEnd()->pd.pos;
        Pos bEndPos = b->bandEnd()->backEnd()->pd.pos;
        if(glm::length(fEndPos - lastPos)<glm::length(bEndPos - lastPos)){
            auto pairVec = b->getGLIndexPairVec(b->bandEnd()->frontEnd(), m_distance);
            lastPos = bEndPos;
            indexPairVecs.push_back(pairVec);
        }
        else{
            auto pairVec = b->getGLIndexPairVec(b->bandEnd()->backEnd(), m_distance);
            lastPos = fEndPos;
            indexPairVecs.push_back(pairVec);
        }
    }
}

void AnimateController::showNext(){
    int nextShowInd = showInd+1;
    int nextBandInd = bandInd+1;
    if(nextShowInd >= indexPairVecs.at(bandInd).size()){
        if(nextBandInd >= bandPtrs.size()){
            ctrl->mainWindow->animationOver();
        }
        else{
            showInd=0;
            ++bandInd;
            bandPtrs.at(bandInd)->setShowRange(indexPairVecs.at(bandInd).at(showInd));
        }
    }
    else{
        ++showInd;
        bandPtrs.at(bandInd)->setShowRange(indexPairVecs.at(bandInd).at(showInd));
    }
    ++windedTotal;
}

void AnimateController::calculation(){
    initBandPtrs();
    initIndexPairVecs();
    for(auto b:bandPtrs){
        b->setShowRange(GLIndexPair{0,0});
    }
    for(auto pairVec:indexPairVecs){
        pairTotal += pairVec.size();
    }
}

int AnimateController::getPercent(){
    int percent = 100.0*windedTotal/pairTotal;
    return percent;
}

void AnimateController::resetBand(){
    for(auto b:bandPtrs){
        b->resetShowRange();
    }
}

void AnimateController::setPercent(int p){
    windedTotal = p*1.0/100*pairTotal;
    int left = windedTotal;
    for(auto b:bandPtrs){
        b->setShowRange({0,0});
    }
    for(int i =0; i < indexPairVecs.size(); ++i){
        auto& pairVec = indexPairVecs.at(i);
        if(left>pairVec.size()){
            left -= pairVec.size();
            bandPtrs.at(i)->setShowRange(indexPairVecs.at(i).back());
        }
        else{
            bandInd = i;
            showInd = left-1;
            if(showInd >=0){
                bandPtrs.at(i)->setShowRange(indexPairVecs.at(i).at(showInd));
            }
            break;
        }
    }
}

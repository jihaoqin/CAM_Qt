#include "AnimateController.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"
#include "mainwindow.h"
#include "EnvelopAssist.h"
#include "Data.h"
#include "HangingBandSet.h"

AnimateController::AnimateController(Controller* c)
    :ctrl(c)
{
    m_distance = 2;
    bandInd = 0;
    showInd = -1;
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
            return;
        }
        else{
            bandPtrs.at(nextBandInd)->setShowRange(indexPairVecs.at(nextBandInd).at(0));
            bandInd = nextBandInd;
            showInd=0;
        }
    }
    else{
        bandPtrs.at(bandInd)->setShowRange(indexPairVecs.at(bandInd).at(nextShowInd));
        showInd = nextShowInd;
    }
    auto root = ctrl->data->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
    ++windedTotal;
    if(windedTotal < pairTotal){
        hangPtr->setShowInd(windedTotal - 1);
    }
    else{
        ctrl->mainWindow->animationOver();
    }
}

void AnimateController::calculation(){
    initBandPtrs();
    if(bandPtrs.size() ==0 ){
        return;
    }
    initIndexPairVecs();
    initHangingBand();
    solveCollision();
    initBandPos();
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
        if(left > pairVec.size()){
            left -= pairVec.size();
            bandPtrs.at(i)->setShowRange(indexPairVecs.at(i).back());
        }
        else{
            bandInd = i;
            showInd = left-1;
            if(showInd >= 0){
                bandPtrs.at(i)->setShowRange(indexPairVecs.at(i).at(showInd));
            }
            else{
                bandPtrs.at(i)->setShowRange(indexPairVecs.at(i).at(0));
                showInd = -1;
            }
            break;
        }
    }
    auto root = ctrl->data->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
    if(windedTotal == 0){
        hangPtr->setShowInd(0);
    }
    else{
        hangPtr->setShowInd(windedTotal - 1);
    }
}

void AnimateController::initHangingBand(){
    auto root = ctrl->data->getNodeRoot();
    auto tee =  std::dynamic_pointer_cast<Tee>(root->findObjectId("tee"));
    EnvelopAssist assist(tee);
    SuperPosVec allSuperPosVec;
    vector<int> crossInds;
    for(int i = 0; i < bandPtrs.size(); ++i){
        GLIndexPairVec& pairVec = indexPairVecs.at(i);
        vector<int> inds;
        BandPtr bPtr = bandPtrs.at(i);
        if(bPtr->beginFlag().contains("front")){
            for(auto p:pairVec){
                inds.push_back(p.second);
            }
        }
        else{
            for(auto p:pairVec){
                inds.push_back(p.first);
            }
        }
        auto pds = bPtr->indexsPds(inds);
        for(auto i = 0; i<pds.size(); ++i){
            auto pd = pds.at(i);
            auto superPosVec = assist.intersectPoint(pd.pos, pd.dir);
            assert(superPosVec.size() == 1);
            if(assist.isCross(pd.pos, superPosVec.at(0).pos)){
                assist.isCross(pd.pos, superPosVec.at(0).pos);
                crossInds.push_back(allSuperPosVec.size());
            }
            std::string where = superPosVec.at(0).meshName;
            allSuperPosVec.push_back(SuperPos{pd.pos, where});
            allSuperPosVec.push_back(superPosVec.at(0));
        }
    }
    HangingBandSetPtr hangPtr;
    if(root->findObjectId("post") == nullptr){
        hangPtr = std::make_shared<HangingBandSet>(allSuperPosVec);
        hangPtr->setCrossIndexs(crossInds);
        QOpenGLContext* c = ctrl->getGLContext();
        hangPtr->bindGL(c);
        ctrl->data->addHang(hangPtr);
    }
    else{
        hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
        hangPtr->setData(allSuperPosVec);
        hangPtr->setCrossIndexs(crossInds);
    }
}


void AnimateController::solveCollision(){
    auto root = ctrl->data->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
    auto tee =  std::dynamic_pointer_cast<Tee>(root->findObjectId("tee"));
    EnvelopAssist assist(tee);
    SuperPosVec poss = hangPtr->data();
    vector<int> deleteInds;
    bool some = false;
    for(int i = poss.size()-1; i > 0; i = i-2){
        SuperPos sendSuper = poss.at(i-1);
        SuperPos getSuper = poss.at(i);
        if(assist.isCross(sendSuper.pos, getSuper.pos)){
            deleteInds.push_back(i-1);
            deleteInds.push_back(i);
            some = true;
        }
        else{
            if(some == true){
                //处理一下delete
                int beginInd = i;
                int endInd = deleteInds.at(1)+2;
                SuperPos beginSuper = poss.at(beginInd);
                SuperPos endSuper = poss.at(endInd);
                SuperPosVec b2e{poss.at(beginInd-1), poss.at(beginInd), poss.at(endInd-1), poss.at(endInd)};
                auto insertSupers = assist.genInsertedSuper(b2e);
                SuperPosVec doubleSupers;
                for(auto s:insertSupers){
                    doubleSupers.push_back(poss.at(beginInd-1));
                    doubleSupers.push_back(s);
                }
                poss.erase(poss.begin()+beginInd+1, poss.begin()+endInd-1);
                poss.insert(poss.begin()+beginInd+1, doubleSupers.begin(), doubleSupers.end());
                for(auto i = (beginInd+1)/2; i <= (endInd-3)/2; i++){
                    auto ij = getInsertInd(i);
                    auto& pairVec = indexPairVecs.at(ij.first);
                    pairVec.erase(pairVec.begin()+ij.second);
                }
                {
                    auto ij = getInsertInd((beginInd-1)/2);
                    auto& pairVec = indexPairVecs.at(ij.first);
                    GLIndexPairVec ps;
                    if(ij.second == 0){
                        for(int i = 0; i<insertSupers.size(); ++i){
                            ps.push_back(pairVec.at(ij.second));
                        }
                    }
                    else{
                        for(int i = 0; i<insertSupers.size(); ++i){
                            ps.push_back(pairVec.at(ij.second-1));
                        }
                    }
                    pairVec.insert(pairVec.begin() + ij.second, ps.begin(), ps.end());
                }
                deleteInds.clear();
                some = false;
            }
        }
    }
    hangPtr->setData(poss);
}


std::pair<int, int> AnimateController::getInsertInd(int ind){
    int i = 0;
    int left = ind+1;
    while(left>=1){
        auto pairVec = indexPairVecs.at(i);
        if(left > pairVec.size()){
            left -= pairVec.size();
            i++;
        }
        else{
            return std::pair<int, int>{i, left - 1};
        }
    }
}


void AnimateController::initBandPos(){
    auto root = ctrl->data->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
    auto tee =  std::dynamic_pointer_cast<Tee>(root->findObjectId("tee"));
    EnvelopAssist assist(tee);
    SuperPosVec poss = hangPtr->data();
    vector<glm::mat4> Ts;
    int possBeginInd = 0;
    for(int i = 0; i < bandPtrs.size(); i++){
        BandPtr band = bandPtrs.at(i);
        auto& pairVec = indexPairVecs.at(i);
        vector<int> inds;
        if(band->beginFlag().contains("front")){
            for(auto p:pairVec){
                inds.push_back(p.second);
            }
        }
        else{
            for(auto p:pairVec){
                inds.push_back(p.first);
            }
        }
        auto pds = band->indexsPds(inds);
        for(int j = 0; j < pairVec.size(); ++j){
            Pos p;
            int ind;
            if(band->beginFlag().contains("front")){
                ind = pairVec.at(j).second;
            }
            else{
                ind = pairVec.at(j).first;
            }
            Dir z = band->outNorm(ind);
            //Dir y = pds.at(j).dir;
            Dir y = glm::normalize(poss.at(possBeginInd+1).pos - poss.at(possBeginInd).pos);
            z = glm::normalize(z - glm::dot(z,y)*y);
            Dir x = glm::normalize(glm::cross(y, z));
            p = band->indexPd(ind).pos;
            glm::mat4 T;
            utility::setPos(T, p);
            utility::setXDir(T, x);
            utility::setYDir(T, y);
            utility::setZDir(T, z);
            Ts.push_back(T);
            possBeginInd += 2;
        }
    }
    hangPtr->setTVec(Ts);
    hangPtr->setShowInd(0);
    vector<float> lengths;
    for(int i = 0; i < poss.size()/2; i = i+1){
        Pos begin = poss.at(2*i).pos;
        Pos end = poss.at(2*i+1).pos;
        lengths.push_back(glm::length(begin - end));
    }
    hangPtr->setHangingLength(lengths);
}


bool AnimateController::hasData(){
    return bandPtrs.size() == 0? false:true;
}

void AnimateController::hideBandSet(){
    auto root = ctrl->data->getNodeRoot();
    HangingBandSetPtr hangPtr = std::dynamic_pointer_cast<HangingBandSet>(root->findObjectId("post"));
    if(hangPtr != nullptr){
        hangPtr->setVisiable(false);
    }
}

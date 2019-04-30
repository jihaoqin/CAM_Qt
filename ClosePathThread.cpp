#include "ClosePathThread.h"
#include "Tee.h"
#include "LeftCylinderAssist.h"
#include "Controller.h"
#include "GeneralBand.h"
#include "Data.h"
#include "Node.h"
#include "Band.h"
using namespace std;

ClosePathThread::ClosePathThread(Controller* c)
    :ctrl(c)
{

}

void ClosePathThread::run(){
    auto data = ctrl->data;
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    vector<LeftCylinderAssist> assistVec;
    assistVec.push_back(LeftCylinderAssist(tee, "left"));
    assistVec.push_back(LeftCylinderAssist(tee, "up"));
    assistVec.push_back(LeftCylinderAssist(tee, "right"));
    int unLinkedNum = 0;
    int ind = 0;
    int leftTotal = -1;
    int leftLeft = -1;
    int upTotal = -1;
    int upLeft = -1;
    int rightTotal = -1;
    int rightLeft = -1;
    auto mtx = data->getMutex();
    emit progress(leftTotal, leftLeft, upTotal, upLeft, rightTotal, rightLeft);
    for(auto& leftAssist:assistVec){
        EndPtrVec couplingEndVec;
        auto children = data->root->childrenPtrVec();
        for(auto c:children){
            DataObjectPtr objPtr = c->getData();
            if(QString(objPtr->getId()).contains("band")){
                BandPtr band = dynamic_pointer_cast<Band>(objPtr);
                BandEndPtr bandEnd = band->bandEnd();
                for(auto end:bandEnd->ends){
                    if(leftAssist.isReturn(end)&& !end->isCoupled()){
                        couplingEndVec.push_back(end);
                    }
                }
            }
        }
        if(ind == 0){
            leftTotal = couplingEndVec.size();
            leftLeft = leftTotal;
        }
        else if(ind == 1){
            upTotal = couplingEndVec.size();
            upLeft = upTotal;
        }
        else{
            rightTotal = couplingEndVec.size();
            rightLeft = rightTotal;
        }
        emit progress(leftTotal, leftLeft, upTotal, upLeft, rightTotal, rightLeft);
        for(auto e:couplingEndVec){
            if(e->isCoupled()){
                continue;
            }
            auto dirEndVec = leftAssist.filterDir(e, couplingEndVec);
            auto cycleEndVec = leftAssist.filterCycle(e, dirEndVec, ctrl->allEnds());
            if(cycleEndVec.size() == 0){
                ++unLinkedNum;
                continue;
            }
            auto innerFirstEndVec = leftAssist.filterInnerFirst(cycleEndVec, ctrl->allEnds());
            auto nearEnd = leftAssist.nearEnd(e, innerFirstEndVec);
            auto tuple1 = leftAssist.genCircleCurve(e, nearEnd);
            auto& pds = get<0>(tuple1);
            auto& strs = get<1>(tuple1);
            mtx->lock();
            auto band = make_shared<GeneralBand>(pds, strs, data->idGenerator.getBandId(), tee, data->bandWidth());
            band->setCouple(e);
            band->setCouple(nearEnd);
            data->addBand(band);
            mtx->unlock();
            if(ind == 0){
                leftLeft -= 2;
            }
            else if(ind == 1){
                upLeft -= 2;
            }
            else{
                rightLeft -= 2;
            }
            emit progress(leftTotal, leftLeft, upTotal, upLeft, rightTotal, rightLeft);
        }
        ++ind;
    }
    assert(unLinkedNum<3);
    reorderPath();
    emit calOver();
}

void ClosePathThread::reorderPath(){
    auto data = ctrl->data;
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    set<End> endsSet = ctrl->allEndsSet();
    if(hasManyEmpty(endsSet)){
        return;
    }
    End end1;
    for(auto ite = endsSet.begin(); ite != endsSet.end();++ite){
        if(ite->nextEndId.isEmpty()){
            end1 = *ite;
            qDebug()<<ite->endId;
            //break;
        }
    }
    vector<End> linkedEnds = {end1};
    End loopE = utility::theOtherEnd(end1, endsSet);
    while(true){
        if(loopE.nextEndId.isEmpty()){
            linkedEnds.push_back(loopE);
            break;
        }
        else{
            End next = utility::getEnd(loopE.nextEndId, endsSet);
            linkedEnds.push_back(loopE);
            linkedEnds.push_back(next);
            loopE = utility::theOtherEnd(next,endsSet);
        }
    }
    QMutex* mtx = data->getMutex();
    vector<NodePtr> sortedBandNodePtrs = {getBandNode(end1.bandId())};
    for(int i =0; i<linkedEnds.size(); i++){
        if(QString(sortedBandNodePtrs.back()->Id()) == (linkedEnds.at(i).bandId())){
            continue;
        }
        else{
            sortedBandNodePtrs.push_back(getBandNode(linkedEnds.at(i).bandId()));
        }
    }
    auto& children = data->root->childrenPtrVec();
    mtx->lock();
    int j = 0;
    for(int i = 0; i<children.size(); i++){
        auto c = children.at(i);
        QString id = c->Id();
        if(id.contains("band")){
            children.at(i) = sortedBandNodePtrs.at(j);
            j++;
        }
    }
    mtx->unlock();
}

NodePtr ClosePathThread::getBandNode(QString bandId){
    vector<NodePtr> bandNodePtrs;
    auto data = ctrl->data;
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    auto root = data->root;
    auto children = root->childrenPtrVec();
    for(auto c:children){
        QString id = c->Id();
        if(id == bandId){
            return c;
        }
    }
}


bool ClosePathThread::hasManyEmpty(std::set<End> endsSet){
    if(endsSet.size() == 0){
        return true;
    }
    int count = 0;
    for(auto ite = endsSet.begin(); ite != endsSet.end();++ite){
        if(ite->nextEndId.isEmpty()){
            count++;
            if(count>2){
                return true;
            }
        }
    }
    return false;
}

#include "ClosePathThread.h"
#include "Tee.h"
#include "LeftCylinderAssist.h"
#include "Controller.h"
#include "GeneralBand.h"
#include "Data.h"
#include "Node.h"
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
                    if(leftAssist.isReturn(end)){
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
            auto band = make_shared<GeneralBand>(pds, strs, data->idGenerator.getBandId(), tee);
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
    emit calOver();
}

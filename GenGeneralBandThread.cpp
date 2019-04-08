#include "GenGeneralBandThread.h"
#include "Controller.h"
#include "Data.h"
#include "Node.h"
#include "glwidget.h"
#include "Tee.h"
#include "LeftCylinderAssist.h"
#include "GeneralBand.h"
#include <memory>
using namespace std;

GenGeneralBandThread::GenGeneralBandThread(QString w, Controller* c)
    :which(w), ctrl(c)
{

}

void GenGeneralBandThread::run(){
    std::shared_ptr<Data> data = ctrl->data;
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    LeftCylinderAssist leftAssist(tee, which);
    auto root = data->root;
    auto children = root->childrenPtrVec();
    PosDirVec beginSide;
    PosDirVec endSide;
    QStringVec beginMesh;
    QStringVec endMesh;
    vector<EndPtr> endVec;
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr b = std::dynamic_pointer_cast<Band>(c->getData());
            BandEndPtr backend = b->bandEnd();
            bool flag1 = leftAssist.isPosAtStart(backend->frontPos())&&backend->frontEnd()->nextEndId.isEmpty();
            if(flag1){
                endVec.push_back(backend->frontEnd());
            }
            bool flag2 = leftAssist.isPosAtStart(backend->backPos())&&backend->backEnd()->nextEndId.isEmpty();
            if(flag2){
                endVec.push_back(backend->backEnd());
            }
        }
    }
    auto mtx = data->getMutex();
    int sum = endVec.size();
    int left = sum;
    emit progress(sum, left);
    for(auto& e:endVec){
        auto tuple1 = leftAssist.genCurve(e);
        auto& pds = get<0>(tuple1);
        auto& strs = get<1>(tuple1);
        mtx->lock();
        GeneralBandPtr band = make_shared<GeneralBand>(pds, strs, data->idGenerator.getBandId(), tee);
        band->setCouple(e);
        data->addBand(band);
        mtx->unlock();
        --left;
        emit progress(sum, left);
    }
    emit calOver();
}

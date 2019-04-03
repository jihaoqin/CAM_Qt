#include "Band.h"

Band::Band()
{

}

void Band::setColor(Color){

}

BandEndPtr Band::bandEnd(){
    return m_end;
}

EndPtr BandEnd::frontEnd(){
    return ends.at(0);
}

EndPtr BandEnd::backEnd(){
    return ends.at(1);
}

Pos BandEnd::frontPos(){
    return ends.at(0)->pd.pos;
}

Pos BandEnd::backPos(){
    return ends.at(1)->pd.pos;
}

void BandEnd::setCouple(EndPtr& other){
    for(auto e:ends){
        if(e->isClose(other)){
            e->nextEndId = other->endId;
            other->nextEndId = e->endId;
            return;
        }
    }
}

QString End::bandId(){
    return endId.split(".").at(0);
}

void Band::setCouple(EndPtr &e){
    m_end->setCouple(e);
}

bool End::isClose(EndPtr &e){
    if(glm::length(e->pd.pos - pd.pos)<1e-1){
        if(abs(abs(glm::dot(e->pd.dir, pd.dir))-1) < 0.2){
            return true;
        }
    }
    return false;
}

BandEnd::BandEnd(QString s, EndPtrVec endVec){
    bandId = s;
    ends = endVec;
}

End::End(PosDir p, QString next, QString now){
    pd = p;
    nextEndId = next;
    endId = now;
}

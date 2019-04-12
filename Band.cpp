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
        if(e->isClose(other)&&glm::dot(e->pd.dir, other->pd.dir)<0){
            e->nextEndId = other->endId;
            other->nextEndId = e->endId;
            return;
        }
    }
    assert(0);
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

void End::setCouple(EndPtr& e){
    nextEndId = e->endId;
    e->nextEndId = endId;
}

void End::setCouple(End& e){
    nextEndId = e.endId;
}

bool End::isCoupled(){
    return nextEndId.isEmpty()?false:true;
}

bool End::operator<(const End& right) const{
    return endId<right.endId;
}
    
QString End::theOtherId(){
    if(endId.contains("front")){
        return bandId()+".back";
    }
    else if(endId.contains("back")){
        return bandId()+".front";
    }
    else{
        assert(0);
    }
}

End::End(){

}

GLIndexPairVec Band::getGLIndexPairVec(EndPtr beginEnd, float dis){
    QString flag;
    if(beginEnd->endId == m_end->frontEnd()->endId){
        flag = "normal";
        std::vector<int> indexs;
        indexs.push_back(0);
        GLIndexPairVec pairVec;
        for(int i=0; i<m_pds.size(); ++i){
            int lastInd = indexs.back();
            Pos lastPos = m_pds.at(lastInd).pos;
            if(glm::length(m_pds.at(i).pos - lastPos)>dis||m_pds.size()-1 == i){
                indexs.push_back(i);
                GLIndexPair loopPair = GLIndexPair{0, i*(m_numPerPd - 1)*6};
                pairVec.push_back(loopPair);
            }
        }
        return pairVec;
    }
    else{
        flag = "reverse";
        std::vector<int> indexs;
        indexs.push_back(m_pds.size()-1);
        GLIndexPairVec pairVec;
        for(int i = m_pds.size() - 1; i >= 0; --i){
            int lastInd = indexs.back();
            Pos lastPos = m_pds.at(lastInd).pos;
            if(glm::length(lastPos - m_pds.at(i).pos) > dis || 0 == i){
                indexs.push_back(i);
                GLIndexPair loopPair = GLIndexPair{i*(m_numPerPd -1)*6, (m_pds.size()-1-i)*(m_numPerPd-1)*6};
                pairVec.push_back(loopPair);
            }
        }
        return pairVec;
    }
}

void Band::setShowRange(GLIndexPair p){
    mesh.setShowRange(p);
    unsigned int beginInd = p.first/(6*(m_numPerPd - 1));
    unsigned int size = p.second/(6*(m_numPerPd - 1))+1;
    for(auto c:curves){
        c->setShowRange(beginInd, size);
    }
}

void Band::resetShowRange(){
    mesh.resetShowRange();
    for(auto c:curves){
        c->resetShowRange();
    }
}

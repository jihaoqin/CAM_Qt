#include "PointSym.h"
#include "Tee.h"

PointSym::PointSym(PointPtr p, QString s, QString flag, TeePtr t):Point(p->getPos(), s.toLatin1().data())
    ,point(p), symFlag(flag), tee(t)
{
    type = "PointSym";
    father.push_back(point);
    updateSelf();
}

void PointSym::updateSelf(){
    auto p = point.lock();
    auto t = tee.lock();
    if(p != nullptr){
        pos = getPos();
        meshName = std::string(t->symmmetryMesh(p->meshId(),symFlag).toLatin1().data());
        Point::updateSelf();
    }
    else{
        assert(0);
    }
}

Pos PointSym::getPos(){
    auto p = point.lock();
    assert(p);
    pos = p->getPos();
    for(auto i=0; i<symFlag.size();i++){
        QString flag = symFlag.at(i);
        if(flag.contains("x",Qt::CaseInsensitive)){
            pos = Pos{pos.x*-1, pos.y, pos.z};
        }
        else if(flag.contains("y", Qt::CaseInsensitive)){
            pos = Pos{pos.x, pos.y*-1, pos.z};
        }
        else if(flag.contains("z", Qt::CaseInsensitive)){
            pos = Pos{pos.x, pos.y, pos.z*-1};
        }
        else{
            assert(0);
        }
    }
    return pos;
}

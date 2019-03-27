#include "DataObject.h"
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
DataObject::DataObject():binded(false), visiable(true),box()
{

}

DataObject::~DataObject(){
}

bool DataObject::isBinded(){
    return binded;
}

void DataObject::setId(const char* c){
    if(id.empty()){
        id = std::string(c);
    }
    else{
        qDebug()<<"Id "<<c<<" has not been set.\n";
    }
}

void DataObject::setVisiable(bool flag){
    visiable = flag;
}

const char* DataObject::getId(){
    return id.c_str();
}

BoundingBox DataObject::boundingBox(){
    return box;
}

void DataObject::addChild(DataObjectPtr d){
    child.push_back(d);
}

void DataObject::updateSelf(){
    assert(0);
}

void DataObject::update(){
    updateSelf();
    updateChild();
}

void DataObject::updateChild(){
    for(auto& i:child){
        i->update();
    }
}

QStringVec DataObject::childId(){
    QStringVec strVec;
    for(auto& i:child){
        if(i){
            QStringVec childIds = i->childId();
            for(auto id:childIds){
                strVec.push_back(id);
            }
            strVec.push_back(i->getId());
        }
    }
    return strVec;
}

QStringVec DataObject::fatherId(){
    QStringVec strVec;
    for(auto& f:father){
        auto i = f.lock();
        if(i){
            QStringVec fatherIds = i->fatherId();
            for(auto id:fatherIds){
                strVec.push_back(id);
            }
            strVec.push_back(i->getId());
        }
    }
    return strVec;
}

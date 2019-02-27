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

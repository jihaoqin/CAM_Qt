#include "Node.h"
#include "DataObject.h"
#include <QDebug>
#include "Tee.h"
#include "Point.h"
#include "Band.h"

Node::Node():father(), type(nothing),data(nullptr)
{

}
Node::Node(DataObjectPtr dataPtr_):father(), type(nothing), data(nullptr)
{
    assert(dataPtr_ != nullptr);
    QString id(dataPtr_->getId());
    if(id.contains("tee")){
        type = tee;
    }
    else if(id.contains("point")){
        type = point;
    }
    else if(id.contains("curve")){
        type = curve;
    }
    else{
        qDebug()<<"Item with wrong type is being added.\n";
    }
    data = dataPtr_;
}
void Node::setFather(Node* papa){
    father = papa;
}

void Node::deleteChild(NodePtr child){
    QString id(child->Id());
    for(auto i = children.begin(); i != children.end();){
        NodePtr item = *i;
        QString itemName(item->Id());
        if(itemName == id){
            item->setFather(nullptr);
            children.erase(i);
        }
    }
}

void Node::deleteChild(const char* c){
    for(int i = 0; i<children.size(); i++){
        NodePtr nodeI = children.at(i);
        if(nodeI != nullptr && nodeI->Id() == QString(c)){
            children.at(i) = nullptr;
            children.erase(children.begin()+i);
            i--;
        }
    }
}

void Node::addChild(NodePtr child){
    child->setFather(this);
    children.push_back(child);
}

const char* Node::Id(){
    return data->getId();
}

BoundingBox Node::boudingBoxUnion(){
    if(type == nothing){
        return BoundingBox();
    }
    BoundingBox result;
    result.OR(data->boundingBox());
    for(auto child:children){
        result.OR(child->boudingBoxUnion());
    }
    return result;
}


DataObjectPtr Node::getData(){
    return data;
}
Node* Node::fatherPtr(){
    return father;
}
std::vector<NodePtr>& Node::childrenPtrVec(){
    return children;
}


Node::~Node(){

}

DataObjectPtr Node::findObjectId(const char *id){
    DataObjectPtr result = nullptr;
    QString nowId(Id());
    if(nowId == QString(id)){
        result = data;
        return result;
    }
    else{
        for(auto child:children){
            DataObjectPtr o = child->findObjectId(id);
            if(o != nullptr){
                result = o;
                return result;
            }
            else{
                //do nothing
            }
        }
    }
    return result;
}

PointPtr Node::findPointPtr(QString id){
    auto basis = findObjectId(id.toLatin1().data());
    if(basis == nullptr){
        return nullptr;
    }
    else{
        return std::dynamic_pointer_cast<Point>(basis);
    }
}

BandPtr Node::findBandPtr(QString id){
    auto basis = findObjectId(id.toLatin1().data());
    if(basis == nullptr){
        return nullptr;
    }
    else{
        return std::dynamic_pointer_cast<Band>(basis);
    }
}


TeePtr Node::findTeePtr(){
    auto basis = findObjectId("tee");
    if(basis == nullptr){
        return nullptr;
    }
    else{
        return std::dynamic_pointer_cast<Tee>(basis);
    }
}

#include "Node.h"
#include "DataObject.h"
#include <QDebug>
#include "Tee.h"

Node::Node():father(nullptr), type(nothing),data(nullptr)
{

}
Node::Node(DataObjectPtr dataPtr_):father(nullptr), type(nothing), data(nullptr)
{
    assert(dataPtr_ != nullptr);
    QString id(dataPtr_->getId());
    if(id.contains("tee")){
        type = tee;
    }
    else if(id.contains("point")){
        type = point;
    }
    else if(id.contains("direction")){
        type = direction;
    }
    else if(id.contains("line")){
        type = line;
    }
    else{
        qDebug()<<"Item with wrong type is being added.\n";
    }
    data = dataPtr_;
}
void Node::setFather(NodePtr papa){
    if(nullptr != father){
        father->deleteChild(static_cast<NodePtr>(this));
    }
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

void Node::addChild(NodePtr child){
    child->setFather(static_cast<NodePtr>(this));
    children.push_back(child);
}

const char* Node::Id(){
    return data->getId();
}

BoundingBox Node::boudingBoxUnion(){
    if(type == direction || type == nothing){
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

NodePtr Node::fatherPtr(){
    return father;
}
std::vector<NodePtr> Node::childrenPtrVec(){
    return children;
}


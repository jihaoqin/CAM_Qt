#ifndef NODE_H
#define NODE_H
#include <vector>
#include <memory>
#include "boundingbox.h"


class Node;
class DataObject;

using DataObjectPtr = std::shared_ptr<DataObject>;
using NodePtr = std::shared_ptr<Node>;

class Node
{
public:
    enum Type{
        tee,
        point,
        direction,
        line,
        nothing
    };
    Node();
    Node(DataObjectPtr);
    BoundingBox boudingBoxUnion();
    NodePtr fatherPtr();
    std::vector<NodePtr> childrenPtrVec();
    void setFather(NodePtr papa);
    void deleteChild(NodePtr child);
    void addChild(NodePtr child);
    DataObjectPtr getData();
    const char* Id();
    template<typename T>
    void dataOperation(T func){
        if(data != nullptr){
            func(data);
        }
        for(auto child:children){
            if(child != nullptr){
                child->dataOperation(func);
            }
        }
    }
private:
    NodePtr father;
    Type type;
    std::vector<NodePtr> children;
    DataObjectPtr data;
};

#endif // NODE_H

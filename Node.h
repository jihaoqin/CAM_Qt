#ifndef NODE_H
#define NODE_H
#include <vector>
#include <memory>
#include "boundingbox.h"


class Node;
class DataObject;

using DataObjectPtr = std::shared_ptr<DataObject>;
using NodePtr = std::shared_ptr<Node>;
using wNodePtr = std::weak_ptr<Node>;

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
    virtual ~Node();
    Node(DataObjectPtr);
    BoundingBox boudingBoxUnion();
    DataObjectPtr findObjectId(const char*);
    Node* fatherPtr();
    std::vector<NodePtr> childrenPtrVec();
    void deleteChild(NodePtr child);
    void addChild(NodePtr child);
    DataObjectPtr getData();
    const char* Id();
    template<typename T>
    void dataOperation(T& func){
        int i = 0;
        bool flag = data != nullptr;
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
    void setFather(Node* papa);
    Node* father;
    Type type;
    std::vector<NodePtr> children;
    DataObjectPtr data;
};

#endif // NODE_H

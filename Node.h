#ifndef NODE_H
#define NODE_H
#include <vector>
#include <memory>
#include "boundingbox.h"
#include "NameDeclare.h"
#include <QString>


class Node;
class DataObject;
class Model;

using DataObjectPtr = std::shared_ptr<DataObject>;
using NodePtr = std::shared_ptr<Node>;
using wNodePtr = std::weak_ptr<Node>;
using HeadPtr = std::shared_ptr<Model>;

class Node
{
public:
    enum Type{
        tee,
        point,
        curve,
        nothing
    };
    Node();
    virtual ~Node();
    Node(DataObjectPtr);
    BoundingBox boudingBoxUnion();
    DataObjectPtr findObjectId(const char*);
    PointPtr findPointPtr(QString);
    BandPtr findBandPtr(QString);
    TeePtr findTeePtr();
    CurvePtr findCurvePtr(QString);
    HeadPtr findHeadPtr();

    Node* fatherPtr();
    std::vector<NodePtr>& childrenPtrVec();
    void deleteChild(NodePtr child);
    void deleteChild(const char* c);
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

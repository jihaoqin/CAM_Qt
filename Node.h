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
    BoundingBox boudingBox();
    void setFather(NodePtr papa);
    void deleteChild(NodePtr child);
    const char* Id();
private:
    NodePtr father;
    Type type;
    std::vector<NodePtr> children;
    DataObjectPtr data;
};

#endif // NODE_H

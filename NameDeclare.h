#ifndef NAMEDECLARE_H
#define NAMEDECLARE_H
#include<memory>
#include <vector>

class Band;
using BandPtr = std::shared_ptr<Band>;
using BandPtrVec = std::vector<BandPtr>;
using GLIndexPair = std::pair<int, int>;
using GLIndexPairVec = std::vector<GLIndexPair>;

class Controller;

class Data;
using DataPtr = std::shared_ptr<Data>;

class Node;
using NodePtr = std::shared_ptr<Node>;

#endif // CLASDECLARE_H

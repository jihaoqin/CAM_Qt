#ifndef NAMEDECLARE_H
#define NAMEDECLARE_H
#include<memory>
#include <vector>
#include <QString>
using QStringVec = std::vector<QString>;

class Band;
using BandPtr = std::shared_ptr<Band>;
using BandPtrVec = std::vector<BandPtr>;
using GLIndexPair = std::pair<int, int>;
using GLIndexPairVec = std::vector<GLIndexPair>;

class Controller;
class GuiConnector;
class TabBackground;
class AnimateController;

class Data;
using DataPtr = std::shared_ptr<Data>;

class Node;
using NodePtr = std::shared_ptr<Node>;
class Tee;
using TeePtr = std::shared_ptr<Tee>;
class Point;
using PointPtr = std::shared_ptr<Point>;
class Curve;
using CurvePtr = std::shared_ptr<Curve>;

#endif // CLASDECLARE_H

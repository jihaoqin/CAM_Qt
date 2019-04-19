#ifndef CLOSEPATHTHREAD_H
#define CLOSEPATHTHREAD_H
#include <QThread>
#include <vector>
#include <set>
#include "Band.h"

class Controller;
class Band;
class Node;
using BandPtr = std::shared_ptr<Band>;
using NodePtr = std::shared_ptr<Node>;

class ClosePathThread:public QThread
{
    Q_OBJECT
public:
    ClosePathThread(Controller* c);
signals:
    void progress(int, int, int, int, int, int);
    void calOver();
protected:
    virtual void run() override;
private:
    void reorderPath();
    bool hasManyEmpty(std::set<End>);
    NodePtr getBandNode(QString);
    Controller* ctrl;
};

#endif // CLOSEPATHTHREAD_H

#ifndef HANGINGBANDSET_H
#define HANGINGBANDSET_H
#include "DataObject.h"
#include "PosDir.h"
#include "Color.h"
#include "HalfPoint.h"


class HangingBandSet:public DataObject
{
public:
    HangingBandSet(SuperPosVec);
    void setData(SuperPosVec);
    virtual void bindGL(QOpenGLContext*);
    virtual void draw(std::shared_ptr<GLProgram>);
    BoundingBox boundingBox();
    void appendCrossIndex(int);
    void setCrossIndexs(std::vector<int>);
    SuperPosVec data();
private:
    void updateBox();
    void bufferData();
    void solveCollision();
    SuperPosVec sendPoints;
    std::vector<int> crossInds;
    
    BoundingBox box;
    Color color;
    unsigned int VAO,VBO,EBO;
    std::vector<unsigned int> indexVec;
    std::vector<Vertex> vertexVec;
    QOpenGLFunctions_4_3_Core *core;
};

using HangingBandSetPtr = std::shared_ptr<HangingBandSet>;

#endif // HANGINGBANDSET_H

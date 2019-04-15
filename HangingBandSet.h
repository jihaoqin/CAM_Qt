#ifndef HANGINGBANDSET_H
#define HANGINGBANDSET_H
#include "DataObject.h"
#include "PosDir.h"
#include "Color.h"


class HangingBandSet:public DataObject
{
public:
    HangingBandSet(PosVec);
    void setData(PosVec);
    virtual void bindGL(QOpenGLContext*);
    virtual void draw(std::shared_ptr<GLProgram>);
    BoundingBox boundingBox();
    void appendCrossIndex(int);
    void setCrossIndexs(std::vector<int>);
private:
    void updateBox();
    void bufferData();
    PosVec sendPoints;
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

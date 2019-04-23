#ifndef HANGINGBANDSET_H
#define HANGINGBANDSET_H
#include "DataObject.h"
#include "PosDir.h"
#include "Color.h"
#include "HalfPoint.h"
#include "Mesh.h"


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
    void setTVec(std::vector<glm::mat4>);
    void setShowInd(int);
    void setHangingLength(vector<float>);
    Pos sendPos(int ind);
    Pos receivePos(int ind);
    glm::mat4 T(int ind);
    int coupleSum();
private:
    void setMesh();
    void updateBox();
    void bufferData();
    void solveCollision();
    SuperPosVec poss;
    std::vector<int> crossInds;
    Mesh mesh;
    float width;
    std::vector<glm::mat4> m_Ts;
    int m_showInd;
    int hangLength;
    vector<GLIndexPair> m_lengths;
    
    BoundingBox box;
    Color color;
    unsigned int VAO,VBO,EBO;
    std::vector<unsigned int> indexVec;
    std::vector<Vertex> vertexVec;
    QOpenGLFunctions_4_3_Core *core;
};

using HangingBandSetPtr = std::shared_ptr<HangingBandSet>;

#endif // HANGINGBANDSET_H

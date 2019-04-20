#ifndef TEE_H
#define TEE_H
#include "DataObject.h"
#include <vector>
#include "vertex.h"
#include "Mesh.h"
#include "Color.h"
#include "Ring.h"
#include "Cylinder.h"
#include "utility.h"
#include "IdGenerator.h"
#include "TriEdgePlane.h"
#include "IdGenerator.h"

using Dir = glm::vec3;
using Pos = glm::vec3;

class GLProgram;
class TeePara{
public:
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;
};

class Tee: public DataObject
{
public:
    friend class LeftCylinderAssist;
    Tee(float , float , float , float ,IdGenerator&);
    virtual ~Tee();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    vector<HalfPoint> intersectionPoints(glm::vec3 pos, glm::vec3 dir);
    vector<HalfPoint> intersectionPointsInPipe(glm::vec3 pos, glm::vec3 dir);
    QStringVec getLeftCylinderId();
    QStringVec getUpCylinderId();
    QStringVec getRightCylinderId();
    QString whichPart(QString);
    BoundingBox boundingBox();
    void setIdUsing(IdGenerator);
    Ring* getRing(QString);
    TeePara teePara();
    TriEdgePlane* getTriPlane(QString);
    Cylinder* getCylinder(QString);
    QString topoValue(QString);
    QString allTopoValue(QString);
    QString cBand2TBandMesh(QString cBandMesh);
    std::map<QString, QString> edgeTopo();
    std::map<QString, QString> allEdgeTopo();
    Dir outNorm(Pos p, QString meshId);
    QString symmmetryMesh(QString , QString);
    glm::mat4 leftMat4();
    glm::mat4 upMat4();
    glm::mat4 rightMat4();

private:
    Mesh generateLeftRing();
    Mesh generateRightRing();
    Mesh generateMainPipe();
    Mesh generateBranchPipe();
    Mesh generateFrontPlane();
    Mesh generateBackPlane();
    Mesh generatePlane(glm::vec3 anchor, std::vector<std::vector<glm::vec3>> edgeVec);
    Mesh generateCylinder(glm::vec3 end_1, glm::vec3 end_2, float r);
    Mesh generateCircle(glm::vec3 anchor, glm::vec3 dir, float r);
    Mesh generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> v, float angle);
    std::vector<Mesh> planeVec;
    std::vector<Ring> ringVec;
    vector<Cylinder> pipeHalfVec;
    std::vector<Cylinder> cylinderVec;
    std::vector<TriEdgePlane> triEdgePlaneVec;
    std::vector<QString> leftCylinderId;
    std::vector<QString> upCylinderId;
    std::vector<QString> rightCylinderId;
    glm::mat4 modelMat;
    Color color;
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;
    std::map<std::pair<QString, QString>, QString> symMap;
    QOpenGLFunctions_4_3_Core *core;

};

#endif // TEE_H

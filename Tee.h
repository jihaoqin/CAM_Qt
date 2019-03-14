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
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include "IdGenerator.h"

class GLProgram;

class Tee: public DataObject
{
public:
    Tee(float , float , float , float );
    virtual ~Tee();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    vector<HalfPoint> intersectionPoints(glm::vec3 pos, glm::vec3 dir);
    BoundingBox boundingBox();
    void setIdUsing(IdGenerator);
    Ring* getRing(QString);
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
    std::vector<Cylinder> cylinderVec;
    glm::mat4 modelMat;
    Color color;
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;

    friend class boost::serialization::access;
    template<typename Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & modelMat & color & pipeR & sideR & lengthMain & lengthBranch;
    }
    template<typename Archive>
    void load(Archive& ar, const unsigned int version){
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & modelMat & color & pipeR & sideR & lengthMain & lengthBranch;
        Mesh up = generateCircle(glm::vec3(0, lengthBranch, 0), glm::vec3(0, 1, 0), pipeR);
        Mesh left = generateCircle(glm::vec3(-1.0*lengthMain/2, 0, 0), glm::vec3(-1, 0, 0),pipeR);
        Mesh right = generateCircle(glm::vec3(1.0*lengthMain/2, 0, 0), glm::vec3(1, 0, 0),pipeR);
        Mesh front = generateFrontPlane();
        Mesh back = generateBackPlane();
        planeVec.push_back(up);
        planeVec.push_back(left);
        planeVec.push_back(right);
        planeVec.push_back(front);
        planeVec.push_back(back);
        Ring ringLeft(sideR+pipeR, pipeR, utility::PI/2,
                    glm::vec3(-(pipeR+sideR), pipeR+sideR,0), glm::vec3(0,0,-1), glm::vec3(1,0,0));
        Ring ringRight(sideR+pipeR, pipeR, utility::PI/2,
                    glm::vec3(pipeR+sideR, pipeR+sideR,0), glm::vec3(0,0,1), glm::vec3(-1,0,0));
        ringVec.push_back(ringLeft);
        ringVec.push_back(ringRight);
        Cylinder branch(glm::vec3(0, pipeR + sideR, 0), glm::vec3(0, lengthBranch, 0), pipeR);
        Cylinder mainPipe(glm::vec3(-lengthMain/2, 0, 0), glm::vec3(lengthMain/2, 0, 0), pipeR);
        cylinderVec.push_back(branch);
        cylinderVec.push_back(mainPipe);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif // TEE_H

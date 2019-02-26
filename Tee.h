#ifndef TEE_H
#define TEE_H
#include "GLBinder.h"
#include <vector>
#include "vertex.h"
#include "Mesh.h"
#include "Color.h"
#include "Ring.h"
#include "Cylinder.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>

class GLProgram;

class Tee: public GLBinder
{
public:
    Tee(float , float , float , float );
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
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
    std::vector<Mesh> meshVec;
    std::vector<Ring> ringVec;
    std::vector<Cylinder> cylinderVec;
    glm::mat4 modelMat;
    Color color;
    BoundingBox box;
    std::shared_ptr<GLProgram> program;
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;

    friend class boost::serialization::access;
    template<typename Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & boost::serialization::base_object<GLBinder>(*this);
        ar & modelMat & color & box & pipeR & sideR & lengthMain & lengthBranch;
    }
    template<typename Archive>
    void load(Archive& ar, const unsigned int version){
        ar & boost::serialization::base_object<GLBinder>(*this);
        ar & modelMat & color & box & pipeR & sideR & lengthMain & lengthBranch;
        Mesh m = generateLeftRing();
        Mesh mR = generateRightRing();
        Mesh mBranch = generateBranchPipe();
        Mesh mMain = generateMainPipe();
        Mesh up = generateCircle(glm::vec3(0, lengthBranch, 0), glm::vec3(0, 1, 0), pipeR);
        Mesh left = generateCircle(glm::vec3(-1.0*lengthMain/2, 0, 0), glm::vec3(-1, 0, 0),pipeR);
        Mesh right = generateCircle(glm::vec3(1.0*lengthMain/2, 0, 0), glm::vec3(1, 0, 0),pipeR);
        Mesh front = generateFrontPlane();
        Mesh back = generateBackPlane();
        meshVec.push_back(m);
        meshVec.push_back(mR);
        meshVec.push_back(mBranch);
        meshVec.push_back(mMain);
        meshVec.push_back(up);
        meshVec.push_back(left);
        meshVec.push_back(right);
        meshVec.push_back(front);
        meshVec.push_back(back);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif // TEE_H

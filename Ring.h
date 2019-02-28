#ifndef RING_H
#define RING_H
#include "Mesh.h"
#include "glm/glm.hpp"
#include <vector>
#include "DataObject.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "utility.h"

class Ring:public DataObject
{
public:
    Ring(double R_, double r_, double angle_ , glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_);
    virtual ~Ring();
    virtual void bindGL(QOpenGLContext *) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
    vector<glm::vec3> intersectionPoints(glm::vec3 begin, glm::vec3 dir);
private:
    vector<Vertex> generateEdge();
    vector<double> generateCoe(glm::vec3 worldPos, glm::vec3 worldDir);
    bool inParaSpace(double theta, double alpha);
    vector<float> paraWithPoint(glm::vec3);
    bool isSamePoint(float theta, float alpha, glm::vec3 pos);
    double R;
    double r;
    double angle;//弧度制
    glm::vec3 anchor;
    glm::vec3 zdir;
    glm::vec3 xdir;
    Mesh m;

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & R & r & angle & anchor & zdir & xdir;
    }
    template<typename Archive>
    void load(Archive& ar, const unsigned int version){
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & R & r & angle & anchor & zdir & xdir;
        vector<Vertex> edge = generateEdge();
        m = utility::generateRevolution(anchor,zdir, edge, angle);
    }
};

#endif // RING_H

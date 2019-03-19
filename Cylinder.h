#ifndef CYLINDER_H
#define CYLINDER_H
#include "DataObject.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include "DataObject.h"
#include "Edge.h"
#include "HalfPoint.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class Cylinder:public DataObject
{
public:
    friend class CylinderAssist;
    friend class CyCurveAssist;
    Cylinder(QString s, glm::vec3 begin_, glm::vec3 end_, double R_, float angle_, glm::vec3 xDir_);
    virtual ~Cylinder();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    vector<HalfPoint> intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir);
    BoundingBox boundingBox();
    vector<EdgePtr> getEdges();
private:
    Mesh& generateCyliner();
    glm::vec3 anchor;
    glm::vec3 zDir;
    glm::vec3 xDir;
    vector<EdgePtr> edges;
    float length;
    float angle;
    float R;
    Mesh m;

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & anchor & zDir & xDir & R & length & angle;
    }
    template<typename Archive>
    void load(Archive& ar, const unsigned int version){
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & anchor & zDir & xDir & R & length & angle;
        m = generateCyliner();
    }
};

#endif // CYLINDER_H

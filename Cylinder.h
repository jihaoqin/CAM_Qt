#ifndef CYLINDER_H
#define CYLINDER_H
#include "DataObject.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include "DataObject.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class Cylinder:public DataObject
{
public:
    Cylinder(glm::vec3 begin_, glm::vec3 end_, double R_);
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
private:
    Mesh& generateCyliner(glm::vec3, glm::vec3, float);
    glm::vec3 begin;
    glm::vec3 end;
    double R;
    Mesh m;

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & begin & end & R;
    }
    template<typename Archive>
    void load(Archive& ar, const unsigned int version){
        ar & boost::serialization::base_object<DataObject>(*this);
        ar & begin & end & R;
        m = generateCyliner(begin, end, R);
    }
};

#endif // CYLINDER_H

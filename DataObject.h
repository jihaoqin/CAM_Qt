#ifndef GLBINDER_H
#define GLBINDER_H

#include <QOpenGLContext>
#include <GLProgram.h>
#include <memory>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "boundingbox.h"



class DataObject
{
public:
    DataObject();
    virtual ~DataObject();
    virtual void bindGL(QOpenGLContext*) = 0;
    virtual void draw(std::shared_ptr<GLProgram>) = 0;
    bool isBinded();
    void setId(const char*);
    const char* getId();
    void setVisiable(bool);
    BoundingBox boundingBox();
protected:
    bool binded;
    bool visiable;
    BoundingBox box;
private:
    std::string id;
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & binded &id &visiable;
    }
};

#endif // OPENGLBINDER_H

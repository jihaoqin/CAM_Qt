#ifndef GLBINDER_H
#define GLBINDER_H

#include <QOpenGLContext>
#include <GLProgram.h>
#include <memory>
#include <string>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "boundingbox.h"
class DataObject;
using DataObjectPtr = std::shared_ptr<DataObject>;
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
    void addChild(DataObjectPtr);
protected:
    virtual void updateChild();
    virtual void update();
    virtual void updateSelf();
    std::vector<DataObjectPtr> child;
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

#ifndef GLBINDER_H
#define GLBINDER_H

#include <QOpenGLContext>
#include <GLProgram.h>
#include <memory>
#include <string>
#include <vector>
#include "boundingbox.h"
#include "rapidjson/prettywriter.h"
class DataObject;
using DataObjectPtr = std::shared_ptr<DataObject>;
using DataObjectWptr = std::weak_ptr<DataObject>;
using QStringVec = std::vector<QString>;
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
    QStringVec childId();
    QStringVec fatherId();
    QString typeStr();
    virtual void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
    }
    virtual void setAnimateT(glm::mat4);
protected:
    virtual void updateChild();
    virtual void update();
    virtual void updateSelf();
    std::vector<DataObjectPtr> child;
    std::vector<DataObjectWptr> father;
    bool binded;
    bool visiable;
    BoundingBox box;
    std::string type;
    glm::mat4 m_animateT;
private:
    std::string id;
};

#endif // OPENGLBINDER_H

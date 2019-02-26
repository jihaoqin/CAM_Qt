#ifndef GLBINDER_H
#define GLBINDER_H

#include <QOpenGLContext>
#include <GLProgram.h>
#include <memory>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>



class GLBinder
{
public:
    GLBinder();
    ~GLBinder();
    virtual void bindGL(QOpenGLContext*) = 0;
    virtual void draw(std::shared_ptr<GLProgram>) = 0;
    bool isBinded();
    void setId(const char*);
    void setVisiable(bool);
protected:
    bool binded;
    bool visiable;
private:
    std::string id;
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & binded &id;
    }
};

#endif // OPENGLBINDER_H

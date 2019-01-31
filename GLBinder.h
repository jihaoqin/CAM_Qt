#ifndef GLBINDER_H
#define GLBINDER_H

#include <QOpenGLContext>
#include <GLProgram.h>
#include <memory>
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
protected:
    bool binded;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & binded;
    }
};

#endif // OPENGLBINDER_H

#include "GLBinder.h"
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
GLBinder::GLBinder():binded(false), visiable(true)
{

}

GLBinder::~GLBinder(){
}

bool GLBinder::isBinded(){
    return binded;
}

void GLBinder::setId(const char* c){
    if(id.empty()){
        id = std::string(c);
    }
    else{
        qDebug()<<"Id "<<c<<" has not been set.\n";
    }
}

void GLBinder::setVisiable(bool flag){
    visiable = flag;
}

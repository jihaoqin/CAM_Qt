#include "GLBinder.h"
#include <QOpenGLFunctions_4_3_Core>
GLBinder::GLBinder():binded(false)
{

}

GLBinder::~GLBinder(){
}

bool GLBinder::isBinded(){
    return binded;
}

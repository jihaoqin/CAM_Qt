#include "GLBinder.h"
#include <QOpenGLFunctions_4_3_Core>
GLBinder::GLBinder()
{

}

GLBinder::~GLBinder(){
    core->glDeleteVertexArrays(1, &VAO);
    core->glDeleteBuffers(1, &VBO);
    core->glDeleteBuffers(1, &EBO);
}

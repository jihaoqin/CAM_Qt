#include "GLProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <QDebug>
#include "config.h"
GLProgram::GLProgram(QOpenGLContext* context, Type t)
{
    core = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(Mesh == t){
        initialProgram(VERTEX_PATH, MESH_FRAGMENT_PATH);
    }
    else if(Point == t){
        initialProgram(VERTEX_PATH, POINT_FRAGMENT_PATH);
    }
    else{
        qDebug()<<"Program type wrong.\n";
    }
}
void GLProgram::setMat4(const char *name, glm::mat4 t){
    GLuint location = uniformLocation(name);
    core->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(t));
}
void GLProgram::setVec3(const char *name, glm::vec3 t){
    GLuint location = uniformLocation(name);
    core->glUniform3fv(location, 1, glm::value_ptr(t));
}

void GLProgram::setVec4(const char *name, glm::vec4 t){
    GLuint location = uniformLocation(name);
    core->glUniform4fv(location, 1, glm::value_ptr(t));
}

void GLProgram::initialProgram(QString vertexPath, QString fragPath){
    if (!addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath)){
        qDebug()<<"Failed to load vertexShader: "<<log()<<"\n";
    }else{
        // do nothing
    }
    if (!addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath)){
        qDebug()<<"Failed to load fragmentShader: "<<log()<<"\n";
    }else{
        // do nothing
    }
    link();
}

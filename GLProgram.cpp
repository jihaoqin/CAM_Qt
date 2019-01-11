#include "GLProgram.h"
#include <glm/gtc/type_ptr.hpp>
GLProgram::GLProgram(QOpenGLContext* context)
{
    core = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
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

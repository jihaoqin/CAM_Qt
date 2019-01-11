#ifndef GLPROGRAM_H
#define GLPROGRAM_H
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>
#include <glm/glm.hpp>

class GLProgram: public QOpenGLShaderProgram
{
public:
    GLProgram(QOpenGLContext*);
    void setMat4(const char*, glm::mat4);
    void setVec3(const char*, glm::vec3);
    void setVec4(const char*, glm::vec4);
private:
    QOpenGLFunctions_4_3_Core* core;
};

#endif // GLPROGRAM_H

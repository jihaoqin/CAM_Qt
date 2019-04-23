#ifndef AXISS_H
#define AXISS_H
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include "Mesh.h"
#include "vertex.h"

class Axiss
{
public:
    Axiss(float xmin, float xmax, float ymin, float ymax);
    void bindGL(QOpenGLContext*);
    void show();
private:
    void initial();
    float m_xmin;
    float m_xmax;
    float m_ymin;
    float m_ymax;
    float m_r;
    Mesh xEnd;
    Mesh yEnd;

    bool binded;
    unsigned int VAO,VBO,EBO;
    std::vector<unsigned int> indexVec;
    VertexVec vertexVec;
    QOpenGLContext* context;
    QOpenGLFunctions_4_3_Core *core;
};

#endif // AXISS_H

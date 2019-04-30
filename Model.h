#ifndef MODEL_H
#define MODEL_H
#include "DataObject.h"
#include "glm/glm.hpp"
#include "Mesh.h"

class Model:public DataObject
{
public:
    Model(Mesh, QString);
    Model();
    void setShowInd(int);
    void setAnimateTs(std::vector<glm::mat4>);
    void setMeshData(vector<Vertex> v, vector<unsigned int> ind);
    virtual void setAnimateT(glm::mat4) override;
public:
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
private:
    std::vector<glm::mat4> m_Ts;
    Mesh m_mesh;
    int m_showInd;
    bool hasMesh;
};

#endif // MODEL_H

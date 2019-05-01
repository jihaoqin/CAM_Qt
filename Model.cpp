#include "Model.h"
#include "Color.h"

Model::Model(Mesh m, QString name)
    :m_mesh(m),m_showInd(0),m_Ts({glm::mat4(1.0)}), hasMesh(false)
{
    visiable = true;
    setId(name.toLatin1().data());
}

Model::Model(){

}


void Model::bindGL(QOpenGLContext* c){
    if(binded  == true || hasMesh == false){
        return ;
    }
    m_mesh.bindGL(c);
    binded = true;
}

void Model::draw(std::shared_ptr<GLProgram> p){
    if(binded == false || visiable == false){
        return ;
    }
    p->setMat4("model", m_Ts.at(m_showInd));
    p->setVec3("material.color", Color::YELLOW);
    m_mesh.draw();
}

void Model::setAnimateT(glm::mat4){
    //屏蔽统一的旋转
    return ;
}

void Model::setShowInd(int ind){
    m_showInd = ind;
}

void Model::setMeshData(vector<Vertex> v, vector<unsigned int> ind){
    m_mesh.setData(v, ind);
    if(v.size()>0){
        hasMesh = true;
    }
}


void Model::setAnimateTs(std::vector<glm::mat4> ts){
    m_Ts = ts;
    m_showInd = 0;
}

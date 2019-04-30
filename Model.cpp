#include "Model.h"
#include "Color.h"

Model::Model(std::vector<Mesh> meshs, QString name)
    :m_meshs(meshs)
{
    setId(name.toLatin1().data());
}

Model::Model(){

}


void Model::bindGL(QOpenGLContext* c){
    if(binded  == true){
        return ;
    }
    for(auto& m:m_meshs){
        m.bindGL(c);
    }
}

void Model::draw(std::shared_ptr<GLProgram> p){
    if(binded == false || visiable == false){
        return ;
    }
    p->setMat4("model", m_Ts.at(m_showInd));
    p->setVec3("material.color", Color::YELLOW);
    for(auto& m: m_meshs){
        m.draw();
    }
}

void Model::setAnimateT(glm::mat4){
    //屏蔽统一的旋转
    return ;
}

void Model::setShowInd(int ind){
    m_showInd = ind;
}

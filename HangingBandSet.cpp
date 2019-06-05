#include "HangingBandSet.h"
#include "utility.h"

HangingBandSet::HangingBandSet(SuperPosVec ps, float w)
    :color(Color::BLUE), width(w), m_hangVisiable(true)
{
    setId("post");
    setData(ps);
    setMesh();
}

void HangingBandSet::bindGL(QOpenGLContext* c){
    if(binded == true){
        return;
    }
    else{
        core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
        core->glGenVertexArrays(1, &VAO);
        core->glGenBuffers(1,&VBO);
        core->glGenBuffers(1, &EBO);
        mesh.bindGL(c);
        bufferData();
        binded = true;
    }
}
    
void HangingBandSet::draw(std::shared_ptr<GLProgram> program){
    if(binded == true && visiable == true){
        program->setMat4("model", m_animateT*glm::mat4(1.0));
        core->glBindVertexArray(VAO);
        core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        if(m_hangVisiable == true){
            program->setVec3("material.color", Color::RED);
            int which = 0;
            for(int i = which; i < which+1; i+=2){
                core->glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*i));
            }
            program->setVec3("material.color", Color::YELLOW);
            for(int i = 0; i < crossInds.size(); i+=2){
                core->glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*crossInds.at(i)));
            }
            program->setVec3("material.color", color.rgb);
            core->glDrawElements(GL_LINES, indexVec.size(), GL_UNSIGNED_INT, 0);
        }
        program->setMat4("model",m_animateT*m_Ts.at(m_showInd));
        program->setVec3("material.color", Color::YELLOW);
        mesh.setShowRange(m_lengths.at(m_showInd));
        mesh.draw();
    }
}

void HangingBandSet::bufferData(){
    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexVec.size(), &(vertexVec[0]), GL_STATIC_DRAW);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexVec.size(), &(indexVec[0]), GL_STATIC_DRAW);

    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
}


void HangingBandSet::setData(SuperPosVec posVec){
    poss = posVec;
    vertexVec.clear();
    for(auto p:poss){
        Vertex v;
        v.vertex = p.pos;
        v.normal = Dir{1, 0, 0};
        v.coordinate = glm::vec2{0,0};
        vertexVec.push_back(v);
    }
    indexVec.clear();
    for(int i = 0; i < poss.size(); ++i){
        indexVec.push_back(i);
    }
    updateBox();
    if(binded == true){
        bufferData();
    }
}


BoundingBox HangingBandSet::boundingBox(){
    return box;
}

void HangingBandSet::updateBox(){
    box = BoundingBox(vertexVec);
}

void HangingBandSet::appendCrossIndex(int ind){
    crossInds.push_back(ind);
}

void HangingBandSet::setCrossIndexs(std::vector<int> inds){
    crossInds = inds;
}

SuperPosVec HangingBandSet::data(){
    return poss;
}


void HangingBandSet::setMesh(){
    std::vector<Vertex> vertexs;
    int num = 1000;
    for(auto i = 0; i < num; i++){
        Vertex v;
        v.vertex = glm::vec3{width/(-2), i, 0};
        v.normal = Dir{0,0,1};
        v.coordinate = glm::vec2{0, 0};
        vertexs.push_back(v);
        v.vertex = Pos{width/2, i, 0};
        vertexs.push_back(v);
    }
    vector<unsigned int> inds;
    for(auto i = 0; i<num-1; i++){
        inds.push_back(2*i);
        inds.push_back(2*i+1);
        inds.push_back(2*i+2);

        inds.push_back(2*i+1);
        inds.push_back(2*i+3);
        inds.push_back(2*i+2);
    }
    mesh = Mesh(vertexs, inds);
}


void HangingBandSet::setTVec(std::vector<glm::mat4> Ts){
    m_Ts = Ts;
}

void HangingBandSet::setShowInd(int ind){
    m_showInd = ind;
}


void HangingBandSet::setHangingLength(vector<float> ls){
    m_lengths.clear();
    for(auto l:ls){
        int intL = l;
        m_lengths.push_back(GLIndexPair{0, intL*6});
    }
}

glm::mat4 HangingBandSet::receiveT(int ind){
    return m_Ts.at(ind);
}

glm::mat4 HangingBandSet::sendT(int ind){
    auto T1 = receiveT(ind);
    utility::setPos(T1, sendPos(ind));
    return T1;
}

Pos HangingBandSet::sendPos(int ind){
    return poss.at(2*ind+1).pos;
}

Pos HangingBandSet::receivePos(int ind){
    return poss.at(2*ind).pos;
}

int HangingBandSet::coupleSum(){
    assert((poss.size()&1) == 0);
    return poss.size()/2;
}

glm::mat4 HangingBandSet::animateT(int ind){
    return rotxs.at(ind);
}

void HangingBandSet::setAnimateTs(vector<glm::mat4> Ts){
    rotxs = Ts;
}

void HangingBandSet::setHangLinesVisiable(bool flag){
    m_hangVisiable = flag;
}

Pos HangingBandSet::currentSendPos(){
    glm::mat4 currentT = m_Ts.at(m_showInd);
    glm::vec3 curRecPos = currentT[3];
    glm::vec3 yDir = currentT[1];
    //glm::vec3 sendPos = curRecPos + m_lengths.at(m_showInd).second*1.0f/6.0f*yDir;
    glm::vec3 sendPos = poss.at(m_showInd*2+1).pos;
    return sendPos;
}

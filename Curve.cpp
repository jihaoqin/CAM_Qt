#include "Curve.h"

Curve::Curve(std::vector<glm::vec3> points, const char* c):color(Color::BLACK)
{
    setId(c);
    initial(points);
    box = BoundingBox(vertexVec);
}
Curve::Curve(const char* c):color(Color::BLACK)
{
    setId(c);
}

void Curve::bindGL(QOpenGLContext *c){
    core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
    core->glGenVertexArrays(1, &VAO);
    core->glBindVertexArray(VAO);
    core->glGenBuffers(1,&VBO);
    core->glGenBuffers(1, &EBO);
    binded = true;
    bufferData();
}

void Curve::bufferData(){
    if(binded == false){
        return;
    }
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

void Curve::draw(std::shared_ptr<GLProgram> program){
    if(binded == false){
        assert(binded == true);
        return;
    }
    core->glDepthMask(GL_FALSE);
    core->glEnable(GL_POLYGON_OFFSET_FILL);
    core->glPolygonOffset(1.0f, 1.0f);
    core->glEnable(GL_BLEND);
    core->glEnable(GL_LINE_SMOOTH);
    core->glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    program->setMat4("model", glm::mat4(1.0));
    program->setVec3("material.color", color.rgb);
    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //core->glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
    core->glDrawArrays(GL_LINE_STRIP, 0, vertexVec.size());
    core->glDepthMask(GL_TRUE);
}

void Curve::data(std::vector<glm::vec3> points){
    initial(points);
    if(binded == true){
        bufferData();
    }
}
void Curve::initial(std::vector<glm::vec3> points){
    vertexVec.clear();
    indexVec.clear();
    for(auto i:points){
        Vertex v;
        v.vertex = i;
        v.normal = glm::vec3{1,0,0};
        v.coordinate = glm::vec2{0,0};
        vertexVec.push_back(v);
    }
    for(int i = 0; i<vertexVec.size(); i++){
        indexVec.push_back(i);
    }
    box = BoundingBox(vertexVec);
}

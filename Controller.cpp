#include "Controller.h"
#include <memory>
using namespace  std;
Controller::Controller()
{

}

void Controller::addModel(std::string str){
    shared_ptr<Model> m = make_shared<Model>(str.c_str());
    data->addModel(m);
}

void Controller::bindGL(QOpenGLContext *c, shared_ptr<GLBinder> obj){
   obj->bindGL(c);
}

void Controller::addLine(){
    shared_ptr<Line> l = make_shared<Line>();
    data->addLine(l);
}
void Controller::draw(){
    for (auto m : data->modelVec){
        program->setMat4("model", m->modelMatrix());
        program->setVec4("color", glm::vec4(1.0f, 0.0f,0.5f, 1.0f));
        m->draw(program);
    }
    for(auto l : data->lineVec){
        l->draw(program);
    }
}
void Controller::initialGLVar(QOpenGLContext *c, std::shared_ptr<GLProgram> proPtr){
    context = c;
    program = proPtr;
    core = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
}

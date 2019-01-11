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
void Controller::draw(std::shared_ptr<GLProgram> program){
    program->setMat4("view", data->camera.viewMatrix());
    program->setMat4("perspective", data->camera.perspectiveMatrix());
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

std::shared_ptr<Camera2> Controller::getCamera(){
    return std::make_shared<Camera2>(data->camera);
}
void Controller::bindData(std::shared_ptr<Data> d){
    data = d;
}

void Controller::updateBoundingBox(){
   vector<BoundingBox> boxVec;
   for(auto m:data->modelVec){
       BoundingBox b = m->boundingBox();
       boxVec.push_back(b);
   }
   data->box = BoundingBox::OrBox(boxVec);
}

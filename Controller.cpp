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

}

#include "TBandOnPoint.h"
#include "TCurveAssist.h"
#include "TeeCurveAssist.h"
#include "Color.h"
#include "Tee.h"
#include "TCurve.h"

using namespace std;
using Dir = glm::vec3;
 TBandOnPoint::TBandOnPoint(PointPtr p,  float w, TCurvePtr tc, QString name, TeePtr t)
     :tee(t), point(p),  tcurve(tc), width(abs(w))
{
     setId(name.toLatin1().data());
     updateSelf();
}

 void TBandOnPoint::updateSelf(){
     //TCurveAssist assist(tee);
     TeeCurveAssist teeAss(tee);
     //auto tuples = assist.genCurve(point->getPos(), tcurve->windAngle(), tcurve->slip(), point->meshId());
     auto tuples = teeAss.genCurve(point->getPos(), tcurve->windAngle(), tcurve->slip(), point->meshId(), 22);
     auto pds = get<0>(tuples);
     auto strs = get<1>(tuples);
     PosVec meshPos;
     vector<Vertex> vertexVec;
     width = 6;
     float h = 1;
     float times = (int)(width/2/h) + 1;
     for(unsigned int i = 0; i < pds.size(); i++){
         Dir norm = tee->outNorm(pds.at(i).pos, strs.at(i));
         Dir right = glm::cross(pds.at(i).dir, norm);
         Dir left = -1.0f*right;
         PosVec rightPos;
         PosVec leftPos;
         rightPos.push_back(pds.at(i).pos);
         for(int i = 1; i < times; i++){
            auto temp = teeAss.genCurve({pds.at(i).pos, right}, 0, strs.at(i), i*h);
            rightPos.push_back(get<0>(temp).back().pos);
         }
         auto temp = teeAss.genCurve({pds.at(i).pos, right}, 0, strs.at(i), width/2);
         rightPos.push_back(get<0>(temp).back().pos);

         for(int i = 1; i < times; i++){
            auto temp = teeAss.genCurve({pds.at(i).pos, left}, 0, strs.at(i), i*h);
            leftPos.push_back(get<0>(temp).back().pos);
         }
         temp = teeAss.genCurve({pds.at(i).pos, left}, 0, strs.at(i), width/2);
         leftPos.push_back(get<0>(temp).back().pos);
         for(auto i = rightPos.rbegin(); i!=rightPos.rend(); i++){
             Vertex v;
             v.vertex = *i;
             v.normal = norm;
             v.coordinate = glm::vec2{0,0};
             vertexVec.push_back(v);
         }
         for(unsigned int i =1; i < leftPos.size(); i++){
             Vertex v;
             v.vertex = leftPos.at(i);
             v.normal = norm;
             v.coordinate = glm::vec2{0,0};
             vertexVec.push_back(v);
         }
         for(auto &i:vertexVec){
             if(glm::length(i.vertex) < 10){
                 assert(0);
             }
         }
     }
     unsigned int num = 2*times - 1;
     vector<unsigned int> indexVec;
     for(unsigned int i = 0; i<pds.size()-1;i++){
         for(unsigned int j = 0; j < num -1; j++){
            indexVec.push_back(num*i + j);
            indexVec.push_back(num*i + j+1);
            indexVec.push_back(num*(i+1) + j);

            indexVec.push_back(num*i + j+1);
            indexVec.push_back(num*(i+1) + j+1);
            indexVec.push_back(num*(i+1) + j);
         }
     }
     mesh.setData(vertexVec, indexVec);
 }

 void TBandOnPoint::bindGL(QOpenGLContext *c){
     if(binded == false){
         mesh.bindGL(c);
         binded = true;
     }
     else{
     }
 }

 void TBandOnPoint::draw(std::shared_ptr<GLProgram> p){
    p->setVec3("material.color", Color::YELLOW);
     if(visiable == true){
         mesh.draw();
     }
 }

 BoundingBox TBandOnPoint::boundingBox(){
     return mesh.boundingBox();
 }

#include "TBandOnPoint.h"
#include "TCurveAssist.h"
#include "TeeCurveAssist.h"

using namespace std;
using Dir = glm::vec3;
 TBandOnPoint::TBandOnPoint(PointPtr p, float uAng_, float coe, TCurvePtr tc, QString name, TeePtr t)
     :tee(t), point(p), uAng(uAng_), tcurve(tc), lambda(coe)
{
     setId(name.toLatin1().data());
}

 void TBandOnPoint::updateSelf(){
     TCurveAssist assist(tee);
     TeeCurveAssist teeAss(tee);
     auto tuples = assist.genCurve(point->getPos(), uAng, lambda, point->meshId());
     auto pds = get<0>(tuples);
     auto strs = get<1>(tuples);
     PosVec meshPos;
     vector<Vertex> vertexVec;
     for(unsigned int i = 0; i < pds.size(); i++){
         Dir norm = tee->outNorm(pds.at(i).pos, strs.at(i));
         Dir right = glm::cross(pds.at(i).dir, norm);
         Dir left = -1.0f*right;
         auto rightTuple = teeAss.genCurve({pds.at(i).pos, right}, 0, strs.at(i), width/2);
         auto leftTuple = teeAss.genCurve({pds.at(i).pos, right}, 0, strs.at(i), width/2);
         PosVec rightPos = obtainFivePoint(rightTuple);
         PosVec leftPos = obtainFivePoint(rightTuple);
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
     }
     unsigned int num = 9;
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
     if(visiable == true){
         mesh.draw();
     }
 }

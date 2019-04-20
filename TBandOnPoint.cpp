#include "TBandOnPoint.h"
#include "TCurveAssist.h"
#include "TeeCurveAssist.h"
#include "Color.h"
#include "Tee.h"
#include "TCurve.h"
#include "TeeCurve.h"

using namespace std;
using Dir = glm::vec3;
 TBandOnPoint::TBandOnPoint(PointPtr p,  float w, TCurvePtr tc, QString name, TeePtr t)
     :tee(t), point(p),  tcurve(tc), width(abs(w)), color(Color::GREEN)
{
     type = "TBandOnPoint";
     father.push_back(weak_ptr<Curve>(tcurve));
     setId(name.toLatin1().data());
     CurvePtr c1 = std::make_shared<Curve>("");
     c1->setColor(Color::BLACK);
     CurvePtr c2 = std::make_shared<Curve>("");
     c2->setColor(Color::BLACK);
     curves.push_back(c1);
     curves.push_back(c2);
     updateSelf();
}

 void TBandOnPoint::updateSelf(){
     TCurveAssist assist(tee);
     auto tuples = assist.genDoubleCurve(point->getPos(), tcurve->windAngle(), tcurve->slip(), point->meshId());
     TeeCurveAssist teeAss(tee);
     //auto tuples = teeAss.genCurve(point->getPos(), tcurve->windAngle(), tcurve->slip(), point->meshId(), 22);
     auto& densePds = get<0>(tuples);
     auto& denseStr = get<1>(tuples);
     vector<Pos> densePos;
     for(auto& p:densePds){
         densePos.push_back(p.pos);
     }
     auto fewInd = utility::filterInterval(densePos, 1);
     vector<PosDir> pds;
     QStringVec strs;
     for(auto i:fewInd){
         pds.push_back(densePds.at(i));
         strs.push_back(denseStr.at(i));
     }
     QString bandId = getId();
     QString frontId = bandId + ".front";
     QString backId = bandId + ".back";
     if(QString(getId()).contains("band30")){
         int a = 4;
     }
     PosDir frontPd = {pds.front().pos, -1.0f*pds.front().dir};
     PosDir backPd = pds.back();
     EndPtr frontEnd = make_shared<End>(frontPd,"", frontId);
     EndPtr backEnd = make_shared<End>(backPd,"", backId);
     m_end = make_shared<BandEnd>(bandId, EndPtrVec{frontEnd, backEnd});
     PosVec meshPos;
     vector<Vertex> vertexVec;
     width = 2;
     float h = 1;
     float sideNum = (int)(width/2/h) + 1;
     m_pds = pds;
     m_strs = strs;
     for(unsigned int i = 0; i < pds.size(); i++){
         Dir norm = tee->outNorm(pds.at(i).pos, strs.at(i));
         m_norms.push_back(norm);
         Dir right = glm::cross(pds.at(i).dir, norm);
         Dir left = -1.0f*right;

         auto rightTuple = teeAss.genCurve({pds.at(i).pos, right}, 0, strs.at(i), width/2);
         PosVec rightPos = utility::filterNum(get<0>(rightTuple), sideNum);

         auto leftTuple = teeAss.genCurve({pds.at(i).pos, left}, 0, strs.at(i), width/2);
         PosVec leftPos = utility::filterNum(get<0>(leftTuple), sideNum);

         for(auto j = rightPos.rbegin(); j!=rightPos.rend(); j++){
             Vertex v;
             v.vertex = *j;
             v.normal = norm;
             v.coordinate = glm::vec2{0,0};
             vertexVec.push_back(v);
         }
         for(unsigned int j =1; j < leftPos.size(); j++){
             Vertex v;
             v.vertex = leftPos.at(j);
             v.normal = norm;
             v.coordinate = glm::vec2{0,0};
             vertexVec.push_back(v);
         }
     }
     unsigned int num = 2*sideNum - 1;
     m_numPerPd = num;
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
     vector<Pos> edgePos_1;
     vector<Pos> edgePos_2;
     for(int i = 0; i<pds.size(); i++){
         edgePos_1.push_back(vertexVec.at(i*num).vertex);
         edgePos_2.push_back(vertexVec.at((i+1)*num-1).vertex);
     }
     auto& c1 = curves.at(0);
     c1->data(edgePos_1);
     auto& c2 = curves.at(1);
     c2->data(edgePos_2);
 }

 void TBandOnPoint::bindGL(QOpenGLContext *c){
     if(binded == false){
        core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
        mesh.bindGL(c);
        binded = true;
        for(auto i:curves){
            i->bindGL(c);
        }
     }
     else{
     }
 }

 void TBandOnPoint::draw(std::shared_ptr<GLProgram> p){
    if(binded == false || visiable == false){
        return;
    }
    p->setVec3("material.color", color.rgb);
    core->glDepthMask(GL_FALSE);
    mesh.draw();
    for(auto i:curves){
        i->draw(p);
    }
    core->glDepthMask(GL_TRUE);
 }

 BoundingBox TBandOnPoint::boundingBox(){
     return mesh.boundingBox();
 }


 void TBandOnPoint::setColor(Color c){
     color = c;
 }

 void TBandOnPoint::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
     writer.StartObject();
     writer.String("type");
     writer.String("TBandOnPoint");
     writer.String("width");
     writer.Double(width);
     writer.String("curve");
     tcurve->serialize(writer);
     writer.EndObject();
 }

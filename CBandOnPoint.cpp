#include "CBandOnPoint.h"
#include "LeftCylinderAssist.h"
#include "Tee.h"
#include "CylinderAssist.h"
#include "TeeCurveAssist.h"
#include <tuple>
using namespace std;

CBandOnPoint::CBandOnPoint(HalfPoint p, float angle, TeePtr t, QString id)
    :m_point(p), windingAngle(angle), tee(t), color(Color::YELLOW)
{
    setId(id.toLatin1().data());
    CurvePtr c1 = std::make_shared<Curve>("");
    c1->setColor(Color::BLACK);
    CurvePtr c2 = std::make_shared<Curve>("");
    c2->setColor(Color::BLACK);
    curves.push_back(c1);
    curves.push_back(c2);
    updateSelf();
}

void CBandOnPoint::updateSelf(){
    QString meshName = m_point.meshName.c_str();
    QString which = tee->whichPart(meshName);
    LeftCylinderAssist assist(tee, which);
    Cylinder* cy = tee->getCylinder(m_point.meshName.c_str());
    CylinderAssist cyAssist(*cy);
    SPara uv = cyAssist.worldToUV(m_point.pos);
    PosDir pd = cyAssist.paraToWorld(CPPara{uv.u, uv.v, windingAngle});
    auto tuple1 = assist.genCurve(pd);
    auto& pds = get<0>(tuple1);
    auto& names = get<1>(tuple1);
    initial(pds, names);
}

void CBandOnPoint::initial(PosDirVec pds, QStringVec names){
    TeeCurveAssist assist(tee);
    vector<Pos> densePos;
    for(auto& pd:pds){
        densePos.push_back(pd.pos);
    }
    auto fewInd = utility::filterInterval(densePos, 1);
     vector<PosDir> fewPds;
     QStringVec fewStrs;
     for(auto i:fewInd){
         fewPds.push_back(pds.at(i));
         fewStrs.push_back(names.at(i));
     }

     PosVec meshPos;
     vector<Vertex> vertexVec;
     width = 2;
     float h = 1;
     float sideNum = (int)(width/2/h) + 1;
     for(unsigned int i = 0; i < fewPds.size(); i++){
         Dir norm = tee->outNorm(fewPds.at(i).pos, fewStrs.at(i));
         m_norms.push_back(norm);
         Dir right = glm::cross(fewPds.at(i).dir, norm);
         Dir left = -1.0f*right;

         auto rightTuple = assist.genCurve({fewPds.at(i).pos, right}, 0, fewStrs.at(i), width/2);
         PosVec rightPos = utility::filterNum(get<0>(rightTuple), sideNum);

         auto leftTuple = assist.genCurve({fewPds.at(i).pos, left}, 0, fewStrs.at(i), width/2);
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
     for(unsigned int i = 0; i<fewPds.size()-1;i++){
         for(unsigned int j = 0; j < num -1; j++){
            indexVec.push_back(num*i + j);
            indexVec.push_back(num*i + j+1);
            indexVec.push_back(num*(i+1) + j);

            indexVec.push_back(num*i + j+1);
            indexVec.push_back(num*(i+1) + j+1);
            indexVec.push_back(num*(i+1) + j);
         }
     }
     QString bandId = getId();
     QString frontId = bandId+".front";
     QString backId = bandId+".back";
     PosDir frontPd = {fewPds.front().pos, glm::dot(fewPds.front().pos-fewPds.at(1).pos, fewPds.front().dir)>0? fewPds.front().dir:(-1.0f*fewPds.front().dir)};
     PosDir backPd = {fewPds.back().pos, glm::dot(fewPds.back().pos - fewPds.at(fewPds.size()-2).pos, fewPds.back().dir)>0? fewPds.back().dir:(-1.0f*fewPds.back().dir)};
     EndPtr frontEnd = make_shared<End>(frontPd, "", frontId);
     EndPtr backEnd = make_shared<End>(backPd, "", backId);
     m_end = make_shared<BandEnd>(bandId, EndPtrVec{frontEnd, backEnd});
     mesh.setData(vertexVec, indexVec);
     vector<Pos> edgePos_1;
     vector<Pos> edgePos_2;
     for(int i = 0; i<fewPds.size(); i++){
         edgePos_1.push_back(vertexVec.at(i*num).vertex);
         edgePos_2.push_back(vertexVec.at((i+1)*num-1).vertex);
     }
     auto& c1 = curves.at(0);
     c1->data(edgePos_1);
     auto& c2 = curves.at(1);
     c2->data(edgePos_2);
     m_pds = fewPds;
}

void CBandOnPoint::bindGL(QOpenGLContext* c){
    if(binded == false){
        core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
        mesh.bindGL(c);
        binded = true;
        for(auto i:curves){
            i->bindGL(c);
        }
        binded = true;
    }
}

void CBandOnPoint::draw(std::shared_ptr<GLProgram> p){
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

void CBandOnPoint::setPara(HalfPoint p, float angle){
    m_point = p;
    windingAngle = angle;
    updateSelf();
}

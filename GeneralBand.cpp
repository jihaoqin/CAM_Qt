#include "GeneralBand.h"
#include "TeeCurveAssist.h"
#include "utility.h"
#include "Tee.h"
#include <tuple>
using namespace std;

GeneralBand::GeneralBand(PosDirVec pds, QStringVec names, QString name, TeePtr t):
    tee(t), color(Color::YELLOW)
{
    setId(name.toLatin1().data());
    initial(pds, names);
}
void GeneralBand::bindGL(QOpenGLContext* c){
     if(binded == false){
        core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
        mesh.bindGL(c);
        binded = true;
        for(auto i:curves){
            i->bindGL(c);
        }
     }
}

void GeneralBand::draw(std::shared_ptr<GLProgram> p){
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

BoundingBox GeneralBand::boundingBox(){
     return mesh.boundingBox();
}

void GeneralBand::setColor(Color c){
    color = c;
}

void GeneralBand::initial(PosDirVec pds, QStringVec names){
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
     CurvePtr c1 = std::make_shared<Curve>("");
     c1->setColor(Color::BLACK);
     c1->data(edgePos_1);
     CurvePtr c2 = std::make_shared<Curve>("");
     c2->setColor(Color::BLACK);
     c2->data(edgePos_2);
     curves.push_back(c1);
     curves.push_back(c2);
     m_pds = fewPds;
     m_strs = fewStrs;
}

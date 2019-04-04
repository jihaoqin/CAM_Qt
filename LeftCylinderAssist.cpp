#include "LeftCylinderAssist.h"
#include "Tee.h"
#include "utility.h"
#include <set>
#include <algorithm>
#include "Cylinder.h"
#include "CyCurveAssist.h"
#include "CylinderAssist.h"
#include <algorithm>
LeftCylinderAssist::LeftCylinderAssist(TeePtr t, QStringVec halfCylinder):tee(t)
{
    utility::setPos(T, Pos{(t->pipeR+t->sideR)*-1, 0, 0});
    utility::setXDir(T, Dir{0,0,1});
    utility::setYDir(T, Dir{0,1,0});
    utility::setZDir(T, Dir{-1, 0, 0});
    r = t->pipeR;
    for(auto s:halfCylinder){
        Cylinder* c =tee->getCylinder(s);
        CylinderAssist assist(*c);
        cylinders.push_back(tee->getCylinder(s));
        m_length = assist.getLength();
    }
}

std::tuple<PosDirVec, QStringVec> LeftCylinderAssist::genCurve(EndPtr e){
    End end = *e;
    float pi = asin(1)*2;
    CPPara p1 = worldToCPPara(end.pd);
    p1 = normPara(p1, glm::vec2{0,1});
    float v3 = tee->lengthMain/2 - tee->pipeR - tee->sideR - 1;
    float v1 = p1.v;//v1 = 0;
    float alpha_3 = p1.uAng<(0.5*pi)? 0:pi;
    float lam = (r*(1/cos(p1.uAng) - 1/cos(alpha_3)))/(v3 - v1);
    //assert(abs(lam)<0.2);
    float boundary = p1.uAng<(0.5*pi)?0:pi;
    vector<float> alphas;
    if(p1.uAng>boundary){
        float s = 0;
        float alpha = p1.uAng;
        while(alpha > boundary){
            alphas.push_back(alpha);
            s = s+0.5;
            alpha = atan(tan(p1.uAng) - lam/r*s);
            alpha = p1.uAng<(0.5*pi)? alpha:alpha+pi;
        }
    }
    else{
        float s = 0;
        float alpha = p1.uAng;
        while(alpha < boundary){
            alphas.push_back(alpha);
            s = s+2;
            alpha = atan(tan(p1.uAng) - lam/r*s);
            alpha = p1.uAng<(0.5*pi)? alpha:alpha+pi;
        }
        alphas.push_back(boundary);
    }
    vector<PosDir> pdVec;
    QStringVec strVec;
    CylinderAssist assist_1(*cylinders.at(0));
    CylinderAssist assist_2(*cylinders.at(1));
    for(auto alpha: alphas){
        float u = p1.u + 1/lam*(lnn(p1.uAng) - lnn(alpha));
        float v = p1.v + 1/lam*r*(1/cos(p1.uAng) - 1/cos(alpha));
        CPPara p{u,v,alpha};
        PosDir pd = paraToWorld(p);
        pdVec.push_back(pd);
        if(assist_1.isOnSurface(pd.pos)){
            strVec.push_back(assist_1.cylinderId());
        }
        else if(assist_2.isOnSurface(pd.pos)){
            strVec.push_back(assist_2.cylinderId());
        }
        else{
            assert(0);
        }
    }
    return tuple<PosDirVec, QStringVec>{pdVec, strVec};
}

PosDir LeftCylinderAssist::paraToWorld(CPPara a){
    float u = a.u;
    float v = a.v;
    float uAng = a.uAng;
    glm::vec3 pos{r*cos(u), r*sin(u), v};
    glm::vec3 dir = tanDir(u, v, cos(uAng), sin(uAng));
    glm::vec3 worldPos = utility::multiply(T, pos, "pos");
    glm::vec3 worldDir = utility::multiply(T, dir, "dir");
    return PosDir{worldPos, worldDir};
}

bool LeftCylinderAssist::isPosIn(Pos p){
    auto cyStrs = tee->getLeftCylinderId();
    vector<Cylinder*> cyVec;
    for (auto s:cyStrs){
        auto cy = tee->getCylinder(s);
        CylinderAssist assist(*cy);
        if(assist.isOnSurface(p)){
            return true;
        }
    }
    return false;
}

void LeftCylinderAssist::findCoupleEnd(int ind, std::vector<BandEnd> endVec){
    /*
    auto& endIni = endVec.at(ind);
    if(isPosIn(endIni.first.pos)){
        QString nextBand = endVec.at(ind).firstBand;
        if(nextBand.isEmpty()){
            for(int i = 0; i<endVec.size(); i++){
                if(i == ind){
                    continue;
                }
                auto cand = endVec.at(i);
                if(isPosIn(cand.first.pos)&&cand.firstBand.isEmpty()){
                    CPPara myself = worldToCPPara(endIni.first);
                    CPPara para1 = worldToCPPara(cand.first);
                    BandEndVec temp = endVec;
                    temp.at(ind).firstBand = cand.bandId+QString(".first");
                    temp.at(i).firstBand = endIni.bandId+QString(".first");
                    if(!hasCycle(ind, temp)){
                        //整理myself和para
                        glm::vec2 dir = glm::vec2{para1.u-myself.u, para1.v - myself.v};
                        normPara(myself, dir);
                        normPara(para1, dir);
                        auto sol = getSol(myself, para1, 0.1);
                        if(sol.size() == 0){
                            return;
                        }
                        else{
                            //生成pos和dir，然后套上相应的meshName，之后筛一下，利用TeeCurve画出band
                        }
                    }
                }
                if(isPosIn(cand.second.pos)&&cand.secondBand.isEmpty()){

                }
            }
        }
    }
    if(isPosIn(endIni.second.pos)){
        QString nextBand = endVec.at(ind).secondBand;
        if(nextBand.isEmpty()){

        }
    }

    */
}


int LeftCylinderAssist::findBand(QString bandId, std::vector<BandEnd> endVec){
    for(int i = 0; i<endVec.size(); i++){
        auto& end = endVec.at(i);
        if(bandId.contains(end.bandId)){
            return i;
        }
    }
    return -1;
}

CPPara LeftCylinderAssist::worldToCPPara(PosDir world){
    glm::mat4 invT = glm::inverse(T);
    Pos localPos = utility::multiply(invT, world.pos, "pos");
    float v = localPos.z;
    float u = atan2(localPos.y, localPos.x);
    Dir uTan = tanDir(u, v, 1, 0);
    Dir vTan = tanDir(u, v, 0, 1);
    Dir localDir = utility::multiply(invT, world.dir, "dir");
    float y = glm::dot(localDir, vTan);
    float x = glm::dot(localDir, uTan);
    float uAng = atan2(y, x);
    return CPPara{u,v,uAng};
}

Dir LeftCylinderAssist::tanDir(float u, float v, float uCoe, float vCoe){
    Dir uTan{-1*sin(u), cos(u), 0};
    Dir vTan{0,0,1};
    Dir d = glm::normalize(uTan*uCoe + vTan*vCoe);
    return d;
}

CPPara LeftCylinderAssist::normPara(CPPara &p1, glm::vec2 dir){
    float pi = asin(1)*2;
    glm::vec2 p1Tan{cos(p1.uAng), sin(p1.uAng)};
    if(glm::dot(dir,p1Tan)<0){
        p1.uAng = p1.uAng + pi;
    }
    while(p1.uAng<-0.5*pi){
        p1.uAng = p1.uAng + 2*pi;
    }
    while(p1.uAng > 1.5*pi){
        p1.uAng = p1.uAng - 1.5*pi;
    }
    return p1;
}

std::vector<Sol> LeftCylinderAssist::getSol(CPPara p1, CPPara p3, float absLam){
    //校验P1,p3
    //1.校验是uAng是否同向
    if(p1.uAng*p3.uAng<=0){
        return vector<Sol>();
    }
    //2.uAng不能相等或者很接近
    if(abs(p1.uAng - p3.uAng)<1e-2){
        return vector<Sol>();
    }
    //3.校验缠绕角条件
    {
        float v1 = p1.v;
        float v31 = v1+r/absLam*(1/cos(p1.uAng) - 1/cos(p3.uAng));
        float v32 = v1+r/(-1*absLam)*(1/cos(p1.uAng) - 1/cos(p3.uAng));
        if(!utility::isIn(p3.v, v31, v32)){
            return SolVec();
        }
    }
    //开始计算
    SolVec sols;
    float v1 = p1.v;
    float v3 = p3.v;
    float alpha_1 = p1.uAng;
    float alpha_3 = p3.uAng;
    float alpha_21 = acos(1/(1/cos(alpha_1)-(v3-v1)*absLam/r));
    float alpha_22 = acos(1/(1/cos(alpha_1)-(v3-v1)*-1*absLam/r));
    vector<float> alpha_2s = utility::sameInterval(alpha_21, alpha_22, 0.2);
    for(int i = 0; i < alpha_2s.size(); i++){
        float alpha_2 = alpha_2s.at(i);
        float A11 = lnn(alpha_1) - lnn(alpha_2);
        float A12 = lnn(alpha_2) - lnn(alpha_3);
        float A21 = r/cos(alpha_1) - r/cos(alpha_2);
        float A22 = r/cos(alpha_2) - r/cos(alpha_3);
        glm::mat2 A;
        A[0][0] = A11;
        A[0][1] = A21;
        A[1][0] = A12;
        A[1][1] = A22;
        if(isBad(A)){
            continue;
        }
        vector<float> dus = getDus(p1, alpha_2, p3, r, absLam);
        float dv = p3.v-p1.v;
        for(auto du:dus){
            glm::vec2 divLams = glm::inverse(A)*glm::vec2{du, dv};
            float lam1 = 1/divLams.x;
            float lam2 = 1/divLams.y;
            if(abs(lam1)<absLam && abs(lam2)<absLam){
                sols.push_back(Sol{lam1, lam2, alpha_2});
            }
        }
    }
    //挑选转角最小的sol
    vector<float> absDus;
    for(auto sol:sols){
        float lam1 = sol.lam1;
        float lam2 = sol.lam2;
        float alpha_2 = sol.alpha;
        float absDu = abs(1/lam1*(lnn(alpha_1) - lnn(alpha_2)) + 1/lam2*(lnn(alpha_2) - lnn(alpha_3)));
        absDus.push_back(absDu);
    }
    auto ind = std::min_element(absDus.begin(), absDus.end()) - absDus.begin();
    return vector<Sol>{sols.at(ind)};
}

float LeftCylinderAssist::lnn(float x){
    return log(abs(1/cos(x)+tan(x)));
}

bool LeftCylinderAssist::isBad(glm::mat2 T){
    glm::vec2 h1{T[0][0], T[0][1]};
    glm::vec2 h2{T[1][0], T[1][1]};
    if(glm::length(h1)<1e-2){
        return true;
    }
    if(glm::length(h2)<1e-2){
        return true;
    }
    return false;
}

std::vector<float> LeftCylinderAssist::getDus(CPPara p1, float a2, CPPara p3, float r, float absLam){
    float pi = asin(1)*2;
    float u1 = p1.u;
    float u2 = p3.u;
    assert((p1.uAng - 0.5*pi)*(p3.uAng - 0.5*pi)>0);
    float absDivLam1_max;
    float absDivLam1_min = 1/absLam;
    float absDivLam2_max;
    float absDivLam2_min = 1/absLam;
    float L = abs(p1.v - p3.v);
    float a1 = p1.uAng;
    float a3 = p3.uAng;
    absDivLam1_max = (L-absDivLam2_min*abs(r/cos(a2) - r/cos(a3))) / abs(r/cos(a1) - r/cos(a2));
    absDivLam2_max = (L- absDivLam1_min*abs(r/cos(a1) - r/cos(a2))) / abs(r/cos(a2) - r/cos(a3));
    float absDu_max = absDivLam1_max*abs(lnn(a1) - lnn(a2)) + absDivLam2_max*abs(lnn(a2) - lnn(a3));
    if(p1.uAng<0.5*pi){
        //du>0
        float absDu_min = p3.u - p1.u;
        if(absDu_min<0){
            absDu_min += 2*pi;
        }
        vector<float> dus;
        float du = absDu_min;
        while(du<absDu_max){
            dus.push_back(du);
            du = du + 2*pi;
        }
        return dus;
    }
    else{
        //du<0
        float absDu_min = p3.u - p1.u;
        if(absDu_min>0){
            absDu_min -= 2*pi;
        }
        vector<float> dus;
        float du = -1*absDu_min;
        while(du>(-1*absDu_max)){
            dus.push_back(du);
            du = du - 2*pi;
        }
        return dus;
    }
}

bool LeftCylinderAssist::isReturn(EndPtr e){
    float pi = asin(1)*2;
    if(!isPosIn(e->pd.pos)){
        return false;
    }
    CPPara para = worldToCPPara(e->pd);
    while(para.uAng > 1.5*pi){
        para.uAng -= 2*pi;
    }
    while(para.uAng < -0.5*pi){
        para.uAng += 2*pi;
    }
    if(abs(para.uAng)<1e-2 || abs(para.uAng-pi)<1e-2){
        return true;
    }
    return false;
}

EndPtrVec LeftCylinderAssist::filterDir(EndPtr mainEnd, EndPtrVec endVec){
    float pi = asin(1)*2;
    EndPtrVec dirEndVec;
    for(auto& end:endVec){
        if(mainEnd->endId == end->endId){
            continue;
        }
        auto mainUAng = worldToCPPara(mainEnd->pd).uAng;
        auto listUAng = worldToCPPara(end->pd).uAng;
        while(mainUAng < -0.5*pi){
            mainUAng += 2*pi;
        }
        while(listUAng < -0.5*pi){
            listUAng += 2*pi;
        }
        if(abs(abs(mainUAng - listUAng) - pi)<1e-2){
            dirEndVec.push_back(end);
        }
    }
    return dirEndVec;
}

EndPtrVec LeftCylinderAssist::filterCycle(EndPtr mainEnd, EndPtrVec listEnds, const EndPtrVec allEnds){
    EndPtrVec cycleEnds;
    for(auto& end:listEnds){
        if(end->endId == mainEnd->endId){
            continue;
        }
        auto checkEnds = utility::valueCopyEndPtrVec(allEnds);
        auto copyMainEnd = utility::findEnd(mainEnd->endId, checkEnds);
        auto copyEnd = utility::findEnd(end->endId, checkEnds);
        copyMainEnd->setCouple(copyEnd);
        if(!utility::hasCycle(checkEnds)){
            cycleEnds.push_back(end);
        }
    }
    return cycleEnds;
}

EndPtr LeftCylinderAssist::nearEnd(EndPtr mainEnd, EndPtrVec listEnds){
    float pi = asin(1)*2;
    vector<float> dis;
    for(auto end:listEnds){
        float angle = endToEndAngle(mainEnd, end);
        dis.push_back(abs(angle));
    }
    int ind = std::min_element(dis.begin(), dis.end()) - dis.begin();
    return listEnds.at(ind);
}

std::tuple<PosDirVec, QStringVec> LeftCylinderAssist::genCircleCurve(EndPtr beginPoint, EndPtr endPoint){
    float angle = endToEndAngle(beginPoint, endPoint);
    CPPara para = worldToCPPara(beginPoint->pd);
    PosDirVec pds;
    QStringVec strVec;
    CylinderAssist assist_1(*cylinders.at(0));
    CylinderAssist assist_2(*cylinders.at(1));
    int num = abs(angle)/0.2;
    num = num>5? num:5;
    for(int i = 0; i < num; i++){
        float u = para.u+angle*i/num;
        CPPara paraI {u, para.v, para.uAng};
        auto pd = paraToWorld(paraI);
        pds.push_back(pd);
        if(assist_1.isOnSurface(pd.pos)){
            strVec.push_back(assist_1.cylinderId());
        }
        else if(assist_2.isOnSurface(pd.pos)){
            strVec.push_back(assist_2.cylinderId());
        }
        else{
            assert(0);
        }
    }
    return std::tuple<PosDirVec, QStringVec>{pds, strVec};
}

float LeftCylinderAssist::endToEndAngle(EndPtr e1, EndPtr e2){
    float pi = asin(1)*2;
    CPPara p1 = worldToCPPara(e1->pd);
    CPPara p2 = worldToCPPara(e2->pd);
    float rotateUAng = p1.uAng;
    while(rotateUAng < -0.5*pi){
        rotateUAng += 2*pi;
    }
    float angle;
    if(abs(rotateUAng) < 1e-2){
        angle = p2.u - p1.u;
        while(angle<0){
            angle += 2*pi;
        }
    }
    else if(abs(rotateUAng - pi)< 1e-2){
        angle = p2.u - p1.u;
        while(angle>0){
            angle -= 2*pi;
        }
    }
    else{
        assert(0);
    }
    return angle;
}

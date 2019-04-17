#include "EnvelopAssist.h"
#include "Tee.h"
#include "utility.h"
#include "complex"
#include <algorithm>
#include "utility.h"

using namespace std;

EnvelopAssist::EnvelopAssist(TeePtr t)
    :tee(t)
{
    m_dis = 10;
    TeePara teePara = tee->teePara();
    glm::mat4 TMain;
    utility::setPos(TMain, Pos{teePara.lengthMain/2+m_dis, 0, 0});
    utility::setXDir(TMain, Dir{0, 0, 1});
    utility::setYDir(TMain, Dir{0, 1, 0});
    utility::setZDir(TMain, Dir{-1, 0, 0});
    main = Pipe{teePara.lengthMain+2*m_dis, TMain, teePara.pipeR+m_dis};
    glm::mat4 TBranch;
    utility::setPos(TBranch, Pos{0, 0, 0});
    utility::setXDir(TBranch, Dir{-1, 0, 0});
    utility::setYDir(TBranch, Dir{0, 0, 1});
    utility::setZDir(TBranch, Dir{0, 1, 0});
    branch = Pipe{teePara.lengthBranch-teePara.pipeR+m_dis, TBranch, teePara.pipeR+m_dis};
    glm::mat4 TMainPipe;
    utility::setPos(TMainPipe, Pos{teePara.lengthMain/2, 0, 0});
    utility::setXDir(TMainPipe, Dir{0, 0, 1});
    utility::setYDir(TMainPipe, Dir{0, 1, 0});
    utility::setZDir(TMainPipe, Dir{-1, 0, 0});
    mainPipe = Pipe{teePara.lengthMain, TMainPipe, teePara.pipeR};
    glm::mat4 TBranchPipe;
    utility::setPos(TBranchPipe, Pos{0, 0, 0});
    utility::setXDir(TBranchPipe, Dir{-1, 0, 0});
    utility::setYDir(TBranchPipe, Dir{0, 0, 1});
    utility::setZDir(TBranchPipe, Dir{0, 1, 0});
    branchPipe = Pipe{teePara.lengthBranch, TBranchPipe,teePara.pipeR};
}

SuperPosVec EnvelopAssist::intersectPoint(Pos worldPos, Dir worldDir){
    SuperPosVec allPoints;
    vector<float> pointsDis;
    auto mainInts = main.interPoints(worldPos, worldDir);
    for(auto inter:mainInts){
        if(!branch.isWorldIn(inter)){
            allPoints.push_back(SuperPos{inter, "main"});
            pointsDis.push_back(glm::length(inter-worldPos));
        }
    }
    auto branchInts = branch.interPoints(worldPos, worldDir);
    for(auto inter:branchInts){
        if(!main.isWorldIn(inter)){
            allPoints.push_back(SuperPos{inter, "branch"});
            pointsDis.push_back(glm::length(inter-worldPos));
        }
    }
    if(pointsDis.size() == 0){
        return SuperPosVec{};
    }
    else{
    	auto ind = std::min_element(pointsDis.begin(), pointsDis.end()) - pointsDis.begin();
        return SuperPosVec{allPoints.at(ind)};
    }
}


bool Pipe::isLocalIn(Pos p){
    if(glm::length(glm::vec2{p.x, p.y})<=r){
        if(p.z>= 0 && p.z<=length){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool Pipe::isWorldIn(Pos world){
    glm::mat4 invT = glm::inverse(T);
    Pos local = utility::multiply(invT, world, "pos");
    return isLocalIn(local);
}

PosVec Pipe::interPoints(Pos worldPos, Dir worldDir){
    glm::mat4 invT = glm::inverse(T);
    Pos localPos = utility::multiply(invT, worldPos, "pos");
    Dir localDir = utility::multiply(invT, worldDir, "dir");
    float dx = localDir.x;
    float dy = localDir.y;
    float px = localPos.x;
    float py = localPos.y;
    std::complex<float> A = dx*dx+dy*dy;
    std::complex<float> B = 2*(px*dx + py*dy);
    std::complex<float> C = px*px + py*py - r*r;
    vector<float> xs;
    if(A.real() != 0){
        complex<float> x1 = (-1.0f*B - sqrt(B*B - 4.0f*A*C))/(2.0f*A);
        complex<float> x2 = (-1.0f*B + sqrt(B*B - 4.0f*A*C))/(2.0f*A);
        vector<complex<float>> cxs{x1, x2};
        for(auto x:cxs){
            if(abs(x.imag()<1e-4)){
                Pos localInt = localPos + x.real()*localDir;
                if(localInt.z>=0&&localInt.z<=length){
                    xs.push_back(x.real());
                }
            }
        }
        if(xs.size() == 2){
            if(abs(xs.at(0)-xs.at(1))<1e-3){
                xs.pop_back();
            }
        }
    }
    if(localDir.z!=0){
        std::vector<float> zs{0, length};
        for(auto z:zs){
            float t = (z-localPos.z)/localDir.z;
            Pos local = localPos + t*localDir;
            if(glm::length(glm::vec2{local.x, local.y}) <= r){
                xs.push_back(t);
            }
        }
    }
    PosVec worldInts;
    for(auto x:xs){
        if(x>0){
            Pos localInt = localPos + x*localDir;
            Pos worldInt = utility::multiply(T, localInt, "pos");
            worldInts.push_back(worldInt);
        }
    }
    return worldInts;
}

bool EnvelopAssist::isCross(Pos end1, Pos end2){
    bool flag1 = mainPipe.isCross(end1, end2);
    bool flag2 = branchPipe.isCross(end1, end2);
    return flag1||flag2;
}

bool Pipe::isCross(Pos end1, Pos end2){
    Dir dir = glm::normalize(end2 - end1);
    PosVec points = interPoints(end1, dir);
    if(points.size() == 2){
        if(glm::length(points.at(0)-points.at(1))<1e-1){
            return false;
        }
        return true;
    }
    else if(points.size() > 2){
        assert(0);
    }
    else{
        return false;
    }
}


SuperPosVec EnvelopAssist::genInsertedSuper(SuperPosVec b2e){
    float pi = asin(1)*2;
    SuperPos beginSuper = b2e.front();
    SuperPos endSuper = b2e.back();
    if(QString(beginSuper.meshName.c_str()) == QString(endSuper.meshName.c_str())){
        return genInsertedInOne(b2e);
    }
    else{
        float dt = pi/8;
        vector<float> ts{0, dt*1, dt*2, dt*3, dt*4, dt*5, dt*6, dt*7, dt*8};
        PosVec middlePos;
        float r = main.r;
        for(auto t:ts){
            Pos temp1{r*Pos{0,0,1}*cos(t) + r*Pos{sqrt(2)/2, sqrt(2)/2, 0}*sin(t)};
            Pos temp2{r*Pos{0, 0, 1} * cos(t) + r*Pos{sqrt(2)/-2, sqrt(2)/2, 0} * sin(t)};
            middlePos.push_back(temp1);
            middlePos.push_back(temp2);
        }
        SuperPosVec middleSupers1;
        for(auto p:middlePos){
            middleSupers1.push_back(SuperPos{p, beginSuper.meshName});
        }
        SuperPosVec middleSupers2;
        for(auto p:middlePos){
            middleSupers2.push_back(SuperPos{p, endSuper.meshName});
        }
        vector<float> lengths;
        for(int i = 0; i <middlePos.size(); i++){
            auto a = genInsertedInOne(SuperPosVec{beginSuper, middleSupers1.at(i)});
            auto b = genInsertedInOne(SuperPosVec{middleSupers2.at(i), endSuper});
            lengths.push_back(utility::length(a) + utility::length(b));
        }
        int ind = min_element(lengths.begin(), lengths.end()) - lengths.begin();
        auto middleSuper1 = middleSupers1.at(ind);
        auto middleSuper2 = middleSupers2.at(ind);
        auto supers1 = genInsertedInOne(SuperPosVec{beginSuper, middleSuper1});
        auto supers2 = genInsertedInOne(SuperPosVec{middleSuper2, endSuper});
        SuperPosVec supers;
        supers.insert(supers.begin(), supers1.begin(), supers1.end());
        supers.insert(supers.end(), supers2.begin()+1, supers2.end());
        return supers;
    }
}

SuperPosVec EnvelopAssist::genInsertedInOne(SuperPosVec b2e){
    SuperPos beginSuper = b2e.front();
    SuperPos endSuper = b2e.back();
    assert(QString(beginSuper.meshName.c_str()) == QString(endSuper.meshName.c_str()));
    QString name = beginSuper.meshName.c_str();
    Pipe pipe = main;
    if(name == "branch"){
        pipe = branch;
    }
    auto super1 = pipe.genInternalSuper(beginSuper, endSuper, true);
    auto super2 = pipe.genInternalSuper(beginSuper, endSuper, false);
    float length1 = 0;
    float length2 = 0;
    for(auto i = 0; i<super1.size(); i++){
        length1 += glm::length(beginSuper.pos - super1.at(i).pos);
        length2 += glm::length(beginSuper.pos - super2.at(2).pos);
    }
    return length1<length2? super1 : super2;
}

SuperPosVec Pipe::genInternalSuper(SuperPos beginSuper, SuperPos endSuper, bool flag){
    float pi = asin(1)*2;
    int dir = flag == true? 1:-1;
    Pos beginLocal = invTrans(beginSuper.pos, "pos");
    Pos endLocal = invTrans(endSuper.pos, "pos");
    SPara beginPara = localPara(beginLocal);
    SPara endPara = localPara(endLocal);
    SuperPosVec internalSupers{beginSuper};
    float Du = endPara.u - beginPara.u;
    float Dv = endPara.v - beginPara.v;
    if(flag == 1){
        while(Du<0){
            Du += 2*pi;
        }
    }
    else{
        while(Du>0){
            Du -= 2*pi;
        }
    }
    float du = Du/5;
    float dv = Dv/5;
    for(int i = 1; i<5; i++){
        SPara p{beginPara.u + du*i, beginPara.v+dv*i};
        auto world = paraToWorld(p);
        internalSupers.push_back(SuperPos{world, beginSuper.meshName});
    }
    internalSupers.push_back(endSuper);
    return internalSupers;
}

Pos Pipe::invTrans(glm::vec3 v, QString flag){
    glm::mat4 invT = glm::inverse(T);
    return utility::multiply(invT, v, flag);
}

SPara Pipe::localPara(Pos local){
    float u = atan2(local.y, local.x);
    float v = local.z;
    return SPara{u, v};
}


Pos Pipe::paraToLocal(SPara p){
    return Pos{r*cos(p.u), r*sin(p.u), p.v};
}

Pos Pipe::paraToWorld(SPara p){
    Pos local = paraToLocal(p);
    return utility::multiply(T, local, "pos");
}

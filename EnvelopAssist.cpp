#include "EnvelopAssist.h"
#include "Tee.h"
#include "utility.h"
#include "complex"
#include <algorithm>
#include "utility.h"

using namespace std;

EnvelopAssist::EnvelopAssist(TeePtr t, EnvelopData d)
    :tee(t)
{
    TeePara teePara = tee->teePara();
    glm::mat4 TMain;
    utility::setPos(TMain, Pos{teePara.lengthMain/2+d.incMLength, 0, 0});
    utility::setXDir(TMain, Dir{0, 0, 1});
    utility::setYDir(TMain, Dir{0, 1, 0});
    utility::setZDir(TMain, Dir{-1, 0, 0});
    main = Pipe{teePara.lengthMain+2*d.incMLength, TMain, teePara.pipeR+d.incPipeR};
    glm::mat4 TBranch;
    utility::setPos(TBranch, Pos{0, 0, 0});
    utility::setXDir(TBranch, Dir{-1, 0, 0});
    utility::setYDir(TBranch, Dir{0, 0, 1});
    utility::setZDir(TBranch, Dir{0, 1, 0});
    branch = Pipe{teePara.lengthBranch-teePara.pipeR+d.incBLength, TBranch, teePara.pipeR+d.incPipeR};
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
    SuperPos beginSuper = b2e.at(1);
    SuperPos endSuper = b2e.at(3);
    if(QString(beginSuper.meshName.c_str()) == QString(endSuper.meshName.c_str())){
        return genInsertedInOne(SuperPosVec{b2e.at(0), b2e.at(1), b2e.at(3)});
    }
    else{
        float dt = pi/8;
        vector<float> ts{0, dt*1, dt*2, dt*3, dt*4, dt*5, dt*6, dt*7, dt*8};
        PosVec middlePos;
        float r = main.r;
        for(auto t:ts){
            Pos temp1{r*Pos{0,0,1}*cos(t) + r*Pos{1, 1, 0}*sin(t)};
            Pos temp2{r*Pos{0, 0, 1} * cos(t) + r*Pos{-1, 1, 0} * sin(t)};
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
            auto a = genInsertedInOne(SuperPosVec{b2e.at(0), beginSuper, middleSupers1.at(i)});
            auto b = genInsertedInOne(SuperPosVec{b2e.at(0), middleSupers2.at(i), endSuper});
            lengths.push_back(utility::length(a) + utility::length(b));
        }
        int ind = min_element(lengths.begin(), lengths.end()) - lengths.begin();
        auto middleSuper1 = middleSupers1.at(ind);
        auto middleSuper2 = middleSupers2.at(ind);
        auto supers1 = genInsertedInOne(SuperPosVec{b2e.at(0), beginSuper, middleSuper1});
        auto supers2 = genInsertedInOne(SuperPosVec{b2e.at(0), middleSuper2, endSuper});
        SuperPosVec supers;
        supers.insert(supers.begin(), supers1.begin(), supers1.end());
        supers.insert(supers.end(), supers2.begin()+1, supers2.end());
        return supers;
    }
}

SuperPosVec EnvelopAssist::genInsertedInOne(SuperPosVec b2e){
    SuperPos specialSuper = b2e.at(0);
    SuperPos beginSuper = b2e.at(1);
    SuperPos endSuper = b2e.at(2);
    assert(QString(beginSuper.meshName.c_str()) == QString(endSuper.meshName.c_str()));
    QString name = beginSuper.meshName.c_str();
    Pipe pipe = main;
    if(name == "branch"){
        pipe = branch;
    }
    auto poss = pipe.genInternalSuper(PosVec{specialSuper.pos, beginSuper.pos, endSuper.pos});
    SuperPosVec supers;
    for(auto p:poss){
        supers.push_back(SuperPos{p, beginSuper.meshName});
    }
    return supers;
}

PosVec Pipe::genInternalSuper(PosVec poss){
    float pi = asin(1)*2;
    Pos specialWorld = poss.at(0);
    Pos beginWorld = poss.at(1);
    Pos endWorld = poss.at(2);
    Pos specialLocal = invTrans(specialWorld, "pos");
    Pos beginLocal = invTrans(beginWorld, "pos");
    Pos endLocal = invTrans(endWorld, "pos");
    SPara beginPara = localPara(beginLocal);
    SPara endPara = localPara(endLocal);
    PosVec internalSupers{beginWorld};
    glm::vec2 basis1_1{beginLocal.x, beginLocal.y};
    glm::vec2 basis1_2{endLocal.x, endLocal.y};
    glm::mat2 TT;
    TT[0][0] = basis1_1.x;
    TT[1][0] = basis1_1.y;
    TT[0][1] = basis1_2.x;
    TT[1][1] = basis1_2.y;
    glm::vec2 basis2_1 = glm::inverse(TT)*glm::vec2(0, 1);
    glm::vec2 basis2_2 = glm::inverse(TT)*glm::vec2(1, 0);
    glm::vec2 special{specialLocal.x, specialLocal.y};
    float coord_x = glm::dot(special,basis2_2);
    float coord_y = glm::dot(special,basis2_1);
    float Du = endPara.u - beginPara.u;
    float Dv = endPara.v - beginPara.v;
    while(Du>pi){
        Du -= 2*pi;
    }
    while(Du<-1*pi){
        Du += 2*pi;
    }
    if(coord_x<0 && coord_y < 0){
        if(Du<0){
            Du += 2*pi;
        }
        else{
            Du -= 2*pi;
        }
    }
    else{
        Du = Du;
    }
    float du = Du/5;
    float dv = Dv/5;
    for(int i = 1; i<5; i++){
        SPara p{beginPara.u + du*i, beginPara.v+dv*i};
        auto world = paraToWorld(p);
        internalSupers.push_back(world);
    }
    internalSupers.push_back(endWorld);
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


bool EnvelopAssist::needOff(SuperPos p, float rOff){
    QString name = p.meshName.c_str();
    if(name.contains("main")){
        Pipe& mainEnvelop = main;
        if(mainEnvelop.needOff(p.pos, mainEnvelop.r+rOff)){
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

SuperPos EnvelopAssist::offedMainPos(SuperPos pos, float rOff){

    return SuperPos{main.offedPos(pos.pos, mainPipe.r+rOff), "main"};
}

bool Pipe::needOff(Pos world, float rLim){
    glm::mat4 invT = glm::inverse(T);
    Pos local = utility::multiply(invT, world, "pos");
    if(abs(local.z)<1e-1 || abs(local.z - length) < 1e-1){
        if(glm::length(glm::vec2{local.x, local.y}) < rLim){
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

Pos Pipe::offedPos(Pos world, float rLim){
    glm::mat4 invT = glm::inverse(T);
    Pos local = utility::multiply(invT, world, "pos");
    float l = glm::length(glm::vec2{local.x, local.y});
    float R = rLim;
    Pos newLocal = Pos{R/l*local.x, R/l*local.y, local.z};
    return utility::multiply(T, newLocal, "pos");
}

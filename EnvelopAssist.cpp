#include "EnvelopAssist.h"
#include "Tee.h"
#include "utility.h"
#include "complex"
#include <algorithm>

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
}

PosVec EnvelopAssist::intersectPoint(Pos worldPos, Dir worldDir){
    PosVec allPoints;
    vector<float> pointsDis;
    auto mainInts = main.interPoints(worldPos, worldDir);
    for(auto inter:mainInts){
        if(!branch.isWorldIn(inter)){
            allPoints.push_back(inter);
            pointsDis.push_back(glm::length(inter-worldPos));
        }
    }
    auto branchInts = branch.interPoints(worldPos, worldDir);
    for(auto inter:branchInts){
        if(!main.isWorldIn(inter)){
            allPoints.push_back(inter);
            pointsDis.push_back(glm::length(inter-worldPos));
        }
    }
    if(pointsDis.size() == 0){
        return PosVec{};
    }
    else{
    	auto ind = std::min_element(pointsDis.begin(), pointsDis.end()) - pointsDis.begin();
        return PosVec{allPoints.at(ind)};
    }
}


bool Pipe::isLocalIn(Pos p){
    if(glm::length(glm::vec2{p.x, p.y})<=r){
        if(p.z>=0 && p.z<=length){
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
        if(abs(x1.imag()) < 1e-2){
            xs.push_back(x1.real());
        }
        if(abs(x2.imag()) < 1e-2){
            xs.push_back(x2.real());
        }
    }
    if(localDir.z!=0){
        xs.push_back((0-localPos.z)/localDir.z);
        xs.push_back((length-localPos.z)/localDir.z);
    }
    PosVec worldInts;
    for(auto x:xs){
        if(x>0){
            Pos worldInt = utility::multiply(T, localPos+x*localDir, "pos");
          	worldInts.push_back(worldInt);
        }
    }
    return worldInts;
}

bool Envelope::isCross(Pos end1, Pos end2){
    bool flag1 = main.isCross(end1, end2);
    bool flag2 = branch.isCross(end1, end2);
    return flag1||flag2;
}

bool Pipe::isCross(Pos end1, Pos end2){
    Dir dir = glm::normalize(end2 - end1);
    PosVec points = interPoints(end1, dir);
    if(points.size() == 2){
        return true;
    }
    else{
        return false;
    }
}

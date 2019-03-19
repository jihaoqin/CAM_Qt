#include "TriEdgePlaneAssist.h"
#include "utility.h"
#include "Edge.h"

using namespace utility;

TriEdgePlaneAssist::TriEdgePlaneAssist(TriEdgePlane& tri)
    :R(tri.R), uDir(tri.uDir), anchor(tri.anchor), norm(tri.norm), id(tri.getId())
{
    glm::normalize(uDir);
    glm::vec3 yDir = glm::cross(norm, uDir);
    glm::normalize(yDir);
    norm = glm::cross(uDir, yDir);
    glm::normalize(norm);

    utility::setPos(T, anchor);
    setXDir(T, uDir);
    setZDir(T, norm);
    setYDir(T, glm::cross(norm, uDir));
}

vector<glm::vec3> TriEdgePlaneAssist::intersectionPoitns(glm::vec3 worldPos, glm::vec3 worldDir){
    glm::normalize(worldDir);
    if(abs(glm::dot(worldDir, norm)) < 1e-2){
        return vector<glm::vec3>();
    }
    float t = glm::dot(norm, anchor-worldPos)/(glm::dot(worldDir, norm));
    glm::vec3 p = worldPos + t * worldDir;
    glm::vec3 localPos = world3DToLocal(p, "pos");
    vector<float> para = local3DToUV(localPos);
    float u = para.at(0);
    float v = para.at(1);
    if(inParaSpace(u, v)){
        localPos = paraToLocal3D(u, v);
        worldPos = local3DToWorld(localPos,"pos");
        return vector<glm::vec3>{worldPos};
    }
    else{
        return vector<glm::vec3>();
    }
}

glm::vec3 TriEdgePlaneAssist::world3DToLocal(glm::vec3 world, const char* c){
    glm::mat4 invT = glm::inverse(T);
    QString s(c);
    if(s.contains("pos")){
        glm::vec3 local = invT*glm::vec4(world, 1);
        return local;
    }
    else if(s.contains("dir")){
        glm::vec3 local = invT*glm::vec4(world, 0);
        return local;
    }
    else{
        assert(0);
    }
}

glm::vec3 TriEdgePlaneAssist::local3DToWorld(glm::vec3 local, const char* c){
    QString s(c);
    glm::vec3 world;
    if(s.contains("pos")){
        world = T*glm::vec4(local, 1);
    }
    else if(s.contains("dir")){
        world = T*glm::vec4(local, 0);
    }
    else{
        assert(0);
    }
    return world;
}

vector<float> TriEdgePlaneAssist::local3DProjectToUV(glm::vec3 pos){
    float u = pos.x;
    float v = pos.y;
    if(abs(u) > R+1e-1){
        assert(0);
    }
    if(v < -1*1e-1){
        assert(0);
    }
    if(v > R + 1e-1){
        assert(0);
    }

    if(u < 0){
        if(u < -1*R){
            u = -1*R;
            v = 0;
        }
        else{
            if(v > R){
                u = 0;
                v = R;
            }
            else{
                float x = u+R;
                float y = v-R;
                if(sqrt(x*x + y*y) < R){
                    u = (R+0.1)*cos(atan2(y,x))-R;
                    v = (R+0.1)*sin(atan2(y,x))+R;
                }
            }
        }
    }
    else{
        if(u > R){
            u = R;
            v = 0;
        }
        else{
            if(v > R){
                u = 0;
                v = R;
            }
            else{
                float x = u-R;
                float y = v-R;
                if(sqrt(x*x + y*y) < R){
                    u = (R+0.1)*cos(atan2(y,x))+R;
                    v = (R+0.1)*sin(atan2(y,x))+R;
                }
            }
        }
    }
    return vector<float>{u,v};
}

bool TriEdgePlaneAssist::inParaSpace(float u, float v){
    auto edges = getEdges();
    for(auto e:edges){
        if(e->isOut(u, v)){
            return false;
        }
    }
    return true;
}

vector<EdgePtr> TriEdgePlaneAssist::getEdges(){
    float r = R;
    float pi = asin(1)*2;
    auto arc1=[r, pi](float u, float v)->bool{
        float x = u-r;
        float y = v-r;
        float l = sqrt(x*x + y*y);
        if(l < r){
            return true;
        }
        float angle = atan2(y, x);
        if(angle >= -0.5*pi || angle == -1*pi){
            return true;
        }
        return false;
    };
    auto arc2 = [r, pi](float u, float v)->bool{
        float x = u+r;
        float y = v-r;
        float l = sqrt(x*x + y*y);
        if(l <= r){
            return true;
        }
        float angle = atan2(y, x);
        if(angle >= 0 || angle <= -0.5*pi){
            return true;
        }
        return false;
    };
    auto line = [](float u, float v)->bool{
        return v<=0? true:false;
    };
    auto e1 = std::make_shared<Edge>(arc1);
    e1->Id(id+"_edge1");
    vector<glm::vec3> ps1;
    for(int i =0; i<10; i++){
        float theta = pi+0.5*pi/9*i;
        float u = R*cos(theta) + R;
        float v = R*sin(theta) + R;
        glm::vec3 localPos = paraToLocal3D(u, v);
        ps1.push_back(local3DToWorld(localPos, "pos"));
    }
    e1->data(ps1);

    auto e2 = std::make_shared<Edge>(arc2);
    e2->Id(id+"_edge2");
    vector<glm::vec3> ps2;
    for(int i =0; i<10; i++){
        float theta = -0.5*pi/9*i;
        float u = R*cos(theta) - R;
        float v = R*sin(theta) + R;
        glm::vec3 localPos = paraToLocal3D(u, v);
        ps2.push_back(local3DToWorld(localPos, "pos"));
    }
    e2->data(ps2);

    auto e3 = std::make_shared<Edge>(line);
    e3->Id(id+"_edge3");
    vector<glm::vec3> ps3;
    local3DToWorld({-1*R, 0, 0},"pos");
    ps3.push_back(local3DToWorld({-1*R, 0, 0},"pos"));
    ps3.push_back(local3DToWorld({R, 0, 0},"pos"));
    e3->data(ps3);
    return vector<EdgePtr>{e1, e2, e3};
}

glm::vec3 TriEdgePlaneAssist::paraToLocal3D(float u, float v){
    return glm::vec3{u, v, 0};
}


CPPara TriEdgePlaneAssist::local3DProjectToPara(glm::vec3 pos, glm::vec3 dir){
    auto uv = local3DProjectToUV(pos);
    float u = uv.at(0);
    float v = uv.at(1);
    float uAng = atan2(dir.y, dir.x);
    return CPPara{u, v, uAng};
}

PosDir TriEdgePlaneAssist::CPParaToLocal(CPPara p){
    glm::vec3 localPos {p.u, p.v, 0};
    glm::vec3 localDir {cos(p.uAng), sin(p.uAng), 0};
    return PosDir{localPos, localDir};
}

vector<float> TriEdgePlaneAssist::local3DToUV(glm::vec3 pos){
    assert(abs(pos.z)<0.1);
    return vector<float>{pos.x, pos.y};
}


glm::vec3 TriEdgePlaneAssist::localTangentDir(float u, float v, double uWeight, double vWeight){
    glm::vec3 dir = glm::normalize(glm::vec3{uWeight, vWeight, 0});
    return dir;
}

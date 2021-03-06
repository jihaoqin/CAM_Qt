#include "Tee.h"
#include <algorithm>
#include "config.h"
#include "HalfPoint.h"
#include "map"
#include "RingAssist.h"
#include "CylinderAssist.h"
#include "TriEdgePlaneAssist.h"
#include "LeftCylinderAssist.h"
#include <utility>

using namespace std;
Tee::Tee(float _lengthMain, float _lengthBranch, float _pipeR, float _sideR, IdGenerator& g)
    :modelMat(glm::mat4(0.99)), color({0.6f, 0.6f, 0.6f}),axiss(lengthMain/-2 - pipeR, lengthMain/2 + pipeR, 0, lengthBranch, 0.2*pipeR),
        lengthMain(_lengthMain), lengthBranch(_lengthBranch), pipeR(_pipeR), sideR(_sideR)
      , symMap()

{
    modelMat[3][3] = 1;
    float pi = asin(1)*2;
    setId("tee");
    Mesh up = generateCircle(glm::vec3(0, lengthBranch - pipeR, 0), glm::vec3(0, 1, 0), pipeR);
    Mesh left = generateCircle(glm::vec3(-1.0*lengthMain/2, 0, 0), glm::vec3(-1, 0, 0),pipeR);
    Mesh right = generateCircle(glm::vec3(1.0*lengthMain/2, 0, 0), glm::vec3(1, 0, 0),pipeR);
    Ring ringLeft(g.getRingId(), sideR+pipeR, pipeR, utility::PI/2,
                    glm::vec3(-(pipeR+sideR), pipeR+sideR,0), glm::vec3(0,0,-1), glm::vec3(1,0,0));
    Ring ringRight(g.getRingId(), sideR+pipeR, pipeR, utility::PI/2,
                    glm::vec3(pipeR+sideR, pipeR+sideR,0), glm::vec3(0,0,1), glm::vec3(-1,0,0));
    Cylinder branch1(g.getCylinderId(), glm::vec3(0, pipeR + sideR, 0),
                    glm::vec3(0, lengthBranch - pipeR, 0), pipeR, pi, glm::vec3{0,0,1});
    Cylinder branch2(g.getCylinderId(), glm::vec3(0, pipeR + sideR, 0),
                    glm::vec3(0, lengthBranch- pipeR, 0), pipeR, pi, glm::vec3{0,0,-1});
    upCylinderId.push_back(branch1.getId());
    upCylinderId.push_back(branch2.getId());
    Cylinder mainPipeLeft1(g.getCylinderId(), glm::vec3(-1*(pipeR+sideR), 0, 0),
                      glm::vec3(lengthMain/-2, 0, 0), pipeR, pi, glm::vec3{0,0,1});
    Cylinder mainPipeLeft2(g.getCylinderId(), glm::vec3(-1*(pipeR+sideR), 0, 0),
                      glm::vec3(lengthMain/-2, 0, 0), pipeR, pi, glm::vec3{0,0,-1});
    leftCylinderId.push_back(mainPipeLeft1.getId());
    leftCylinderId.push_back(mainPipeLeft2.getId());
    Cylinder pipeHalf(g.getCylinderId(), glm::vec3(-1*(pipeR+sideR),0,0),
                      glm::vec3{pipeR+sideR, 0, 0}, pipeR, pi, glm::vec3{0,0,1});
    Cylinder mainPipeRight1(g.getCylinderId(), glm::vec3(pipeR+sideR, 0, 0),
                      glm::vec3(lengthMain/2, 0, 0), pipeR, pi, glm::vec3{0,0,1});
    Cylinder mainPipeRight2(g.getCylinderId(), glm::vec3(pipeR+sideR, 0, 0),
                      glm::vec3(lengthMain/2, 0, 0), pipeR, pi, glm::vec3{0,0,-1});
    rightCylinderId.push_back(mainPipeRight1.getId());
    rightCylinderId.push_back(mainPipeRight2.getId());
    TriEdgePlane plane1(g.getPlaneId(), pipeR+sideR, glm::vec3{0,0,pipeR}, glm::vec3{1,0,0}, glm::vec3{0,0,1});
    TriEdgePlane plane2(g.getPlaneId(), pipeR+sideR, glm::vec3{0,0,-1*pipeR}, glm::vec3{-1,0,0}, glm::vec3{0,0,-1});

    ringVec.push_back(ringLeft);
    ringVec.push_back(ringRight);
    symMap[pair<QString,QString>(QString(ringLeft.getId()), QString("x"))] = QString(ringRight.getId());
    symMap[pair<QString,QString>(QString(ringLeft.getId()), QString("z"))] = QString(ringLeft.getId());
    symMap[pair<QString,QString>(QString(ringRight.getId()), QString("x"))] = QString(ringLeft.getId());
    symMap[pair<QString,QString>(QString(ringRight.getId()), QString("z"))] = QString(ringRight.getId());

    cylinderVec.push_back(branch1);
    cylinderVec.push_back(branch2);
    cylinderVec.push_back(mainPipeLeft1);
    cylinderVec.push_back(mainPipeLeft2);
    cylinderVec.push_back(pipeHalf);
    symMap[pair<QString,QString>(QString(pipeHalf.getId()), QString("x"))] = QString(pipeHalf.getId());
    symMap[pair<QString,QString>(QString(pipeHalf.getId()), QString("z"))] = QString(pipeHalf.getId());
    cylinderVec.push_back(mainPipeRight1);
    cylinderVec.push_back(mainPipeRight2);
    pipeHalfVec.push_back(pipeHalf);
    planeVec.push_back(up);
    planeVec.push_back(left);
    planeVec.push_back(right);
    triEdgePlaneVec.push_back(plane1);
    triEdgePlaneVec.push_back(plane2);
    symMap[pair<QString,QString>(QString(plane1.getId()), QString("x"))] = QString(plane1.getId());
    symMap[pair<QString,QString>(QString(plane1.getId()), QString("z"))] = QString(plane2.getId());
    symMap[pair<QString,QString>(QString(plane2.getId()), QString("x"))] = QString(plane2.getId());
    symMap[pair<QString,QString>(QString(plane2.getId()), QString("z"))] = QString(plane1.getId());

    //包围盒
    vector<BoundingBox> boxVec;
    for (auto mesh:planeVec){
        boxVec.push_back(mesh.boundingBox());
    }
    for (auto r:ringVec){
        boxVec.push_back(r.boundingBox());
    }
    for (auto c:cylinderVec){
        boxVec.push_back(c.boundingBox());
    }
    box = BoundingBox::OrBox(boxVec);
    edgeTopo();
}

Mesh Tee::generateLeftRing(){
    glm::vec3 leftAnchor(-1*(pipeR+sideR), pipeR+sideR, 0);
    glm::vec3 leftDir(0, 0, 1);
    float leftAngle = -1*utility::PI/2;
    vector<Vertex> edge;
    int COLUMN = 180;
    for(int i = 0; i < COLUMN+1; i++){
        Vertex v;
        float y = pipeR + sideR;
        float theta = -1*utility::PI/(COLUMN)*i;
        float z = pipeR * cos(theta);
        float x = pipeR * sin(theta);
        v.vertex = glm::vec3(x, y, z);
        v.normal = glm::vec3(x, 0, z);
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    Mesh m = generateRevolution(leftAnchor, leftDir, edge, leftAngle);
    return m;
}

Mesh Tee::generateRightRing(){
    glm::vec3 rightAnchor((pipeR+sideR), pipeR+sideR, 0);
    glm::vec3 rightDir(0, 0, 1);
    float rightAngle = utility::PI/2;
    vector<Vertex> edge;
    int COLUMN = 180;
    for(int i = 0; i < COLUMN+1; i++){
        Vertex v;
        float y = pipeR + sideR;
        float theta = utility::PI/(COLUMN)*i;
        float z = pipeR * cos(theta);
        float x = pipeR * sin(theta);
        v.vertex = glm::vec3(x, y, z);
        v.normal = glm::vec3(x, 0, z);
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    Mesh m = generateRevolution(rightAnchor, rightDir, edge, rightAngle);
    return m;
}
void Tee::bindGL(QOpenGLContext * c){
    if(true == binded){
        qDebug()<<"Tee has been binded.\n";
        return;
    }
    core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
    for(unsigned int i = 0; i < planeVec.size(); i++){
       planeVec.at(i).bindGL(c);
    }
    for(unsigned int i = 0; i < ringVec.size(); i++){
        ringVec.at(i).bindGL(c);
    }
    for(unsigned int i = 0; i < cylinderVec.size(); i++){
        cylinderVec.at(i).bindGL(c);
    }
    for(unsigned int i = 0; i<triEdgePlaneVec.size(); i++){
        triEdgePlaneVec.at(i).bindGL(c);
    }
    axiss.bindGL(c);
    binded = true;
}

void Tee::draw(std::shared_ptr<GLProgram> p){
    if(binded == false || visiable == false){
        return;
    }
    p->setMat4("model", m_animateT*modelMat);
    p->setVec3("material.color", color.rgb);
    core->glDepthMask(GL_TRUE);
    for(unsigned int i = 0; i < planeVec.size(); i++){
       planeVec.at(i).draw();
    }
    for(unsigned int i = 0; i < ringVec.size(); i++){
        ringVec.at(i).draw(p);
    }
    for(unsigned int i = 0; i < cylinderVec.size(); i++){
        cylinderVec.at(i).draw(p);
    }
    for(unsigned int i = 0; i<triEdgePlaneVec.size(); i++){
        triEdgePlaneVec.at(i).draw(p);
    }
    p->setMat4("model", glm::mat4(1.0f));
    axiss.draw(p);
}

Mesh Tee::generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> edge, float angle){
    vector<Vertex> verVec;
    int COLUMN = edge.size()-1;
    int ROW = 100;
    for (int i = 0; i< ROW+1; i++){
        float alpha_i = angle/ROW*i;
        for (auto p:edge){
            Vertex pNew = utility::lineRotateVertex(anchor, dir, p, alpha_i);
            verVec.push_back(pNew);
        }
    }
    auto INDEX = [COLUMN](int i, int j)->int{return i+(COLUMN+1)*j;};
    vector<unsigned int> indexVec;
    for (int i =0; i<COLUMN; i++){
        for(int j = 0; j< ROW; j++){
            //可能是normal出现了问题
            indexVec.push_back(INDEX(i,j));
            indexVec.push_back(INDEX(i+1, j));
            indexVec.push_back(INDEX(i, j+1));

            indexVec.push_back(INDEX(i+1, j));
            indexVec.push_back(INDEX(i+1, j+1));
            indexVec.push_back(INDEX(i, j+1));
        }
    }
    //可以优化成return Mesh(verVec, indexVec);
    Mesh m(verVec, indexVec);
    return m;
}

Mesh Tee::generateCylinder(glm::vec3 end_1, glm::vec3 end_2, float r){
    vector<Vertex> edge;
    int COLUMN = 180;
    glm::vec3 dir = end_2 - end_1;
    glm::vec3 a;
    if(!utility::isParallel(dir, glm::vec3(dir.y, -1*dir.x, dir.z))){
       a = glm::vec3(dir.y, -1*dir.x, dir.z);
    }
    if(!utility::isParallel(dir, glm::vec3(dir.z, dir.y, -1*dir.x))){
       a = glm::vec3(dir.z, dir.y, -1*dir.x);
    }
    glm::vec3 rand = dir + a;
    auto c = glm::cross(dir, rand);
    glm::vec3 radiusVec = glm::normalize(glm::cross(dir, rand)) * r;
    glm::vec3 end_1_new = end_1 + radiusVec;
    for(int i = 0; i< COLUMN+1; i++){
        Vertex v;
        glm::vec3 p = end_1_new + dir*(float)(1.0*i/COLUMN);
        glm::vec3 norm = glm::normalize(radiusVec);
        v.vertex = p;
        v.normal = norm;
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    Mesh m = generateRevolution(end_1, end_2 - end_1, edge, 2*utility::PI);
    return m;
}

Mesh Tee::generateBranchPipe(){
    glm::vec3 end_1(0, pipeR + sideR, 0);
    glm::vec3 end_2 = glm::vec3(end_1.x, lengthBranch, end_1.z);
    Mesh m = generateCylinder(end_1, end_2, pipeR);
    return m;
}

Mesh Tee::generateMainPipe(){
    float sideLength = lengthMain/2;
    glm::vec3 end_1(-1*sideLength, 0, 0);
    glm::vec3 end_2(sideLength, 0, 0);
    Mesh m = generateCylinder(end_1, end_2, pipeR);
    return m;
}

Mesh Tee::generateCircle(glm::vec3 anchor, glm::vec3 dir, float r){
    glm::vec3 a;
    if(!utility::isParallel(dir, glm::vec3(dir.y, -1*dir.x, dir.z))){
       a = glm::vec3(dir.y, -1*dir.x, dir.z);
    }
    if(!utility::isParallel(dir, glm::vec3(dir.z, dir.y, -1*dir.x))){
       a = glm::vec3(dir.z, dir.y, -1*dir.x);
    }
    glm::vec3 rand = dir + a;
    glm::vec3 radiusVec_1 = glm::normalize(glm::cross(dir, rand)) * r;
    glm::vec3 radiusVec_2 = glm::normalize(glm::cross(dir, radiusVec_1)) * r;

    Vertex point;
    point.vertex = anchor;
    point.normal = dir;
    point.coordinate = glm::vec2(0);
    vector<Vertex> verVec;
    verVec.push_back(point);
    int NUM = 180;
    for (int i = 0; i<NUM+1; i++){
        Vertex p;
        float theta = 2*utility::PI/NUM*i;
        auto t = anchor + radiusVec_1*cos(theta) + radiusVec_2 * sin(theta);
        p.vertex = t;
        p.normal = dir;
        p.coordinate = glm::vec2(0);
        verVec.push_back(p);
    }
    vector<unsigned int> indexVec;
    for (int i = 1; i<NUM+1; i++){
        indexVec.push_back(0);
        indexVec.push_back(i);
        indexVec.push_back(i+1);
    }
    Mesh m(verVec, indexVec);
    return m;
}

Mesh Tee::generatePlane(glm::vec3 anchor, vector<vector<glm::vec3>> edgeVec){
    vector<Vertex> verVec;
    for(int i = 0; i < edgeVec.size() - 1; i++){
        vector<glm::vec3> &edge = edgeVec.at(i);
        for(int j = 0; j<edge.size() - 1; j++){
            Vertex v1;
            Vertex v2;
            Vertex v3;
            v1.vertex = anchor;
            glm::vec3 normal = glm::normalize(glm::cross(edge.at(j) - anchor, edge.at(j+1) - anchor));
            v1.normal = normal;
            v1.coordinate = glm::vec2(0);

            v2.vertex = edge.at(j);
            v2.normal = normal;
            v2.coordinate = glm::vec2(0);

            v3.vertex = edge.at(j+1);
            v3.normal = normal;
            v3.coordinate = glm::vec2(0);

            verVec.push_back(v1);
            verVec.push_back(v2);
            verVec.push_back(v3);
        }
    }
    vector<unsigned int> indexVec;
    for(int i = 0; i<verVec.size(); i++){
        indexVec.push_back(i);
    }
    return Mesh(verVec, indexVec);
}

Mesh Tee::generateFrontPlane(){
    glm::vec3 anchor(0, 0, pipeR);
    vector<vector<glm::vec3>> edgeVec;
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    vector<glm::vec3> &edge_1 = edgeVec.at(0);
    vector<glm::vec3> &edge_2 = edgeVec.at(1);
    vector<glm::vec3> &edge_3 = edgeVec.at(2);
    int NUM = 50;
    for (int i = 0; i<NUM +1;i++){
        float theta = utility::PI/2*(-1)/NUM * i;
        glm::vec3 p =glm::vec3(sideR + pipeR,0,0) * cos(theta) + glm::vec3(0,sideR + pipeR,0) * sin(theta) + glm::vec3(-1*(pipeR + sideR), pipeR + sideR,anchor.z);
        edge_1.push_back(p);
        edge_2.push_back(glm::vec3(p.x*(-1), p.y, p.z));
    }
    reverse(edge_2.begin(), edge_2.end());
    edge_3.push_back(edge_1.back());
    edge_3.push_back(edge_2.front());
    return generatePlane(anchor, edgeVec);
}

Mesh Tee::generateBackPlane(){
    glm::vec3 anchor(0, 0, -1*pipeR);
    vector<vector<glm::vec3>> edgeVec;
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    vector<glm::vec3> &edge_1 = edgeVec.at(0);
    vector<glm::vec3> &edge_2 = edgeVec.at(1);
    vector<glm::vec3> &edge_3 = edgeVec.at(2);
    int NUM = 50;
    for (int i = 0; i<NUM +1;i++){
        float theta = utility::PI/2*(-1)/NUM * i;
        glm::vec3 p =glm::vec3(sideR + pipeR,0,0) * cos(theta) + glm::vec3(0,sideR + pipeR,0) * sin(theta) + glm::vec3(-1*(pipeR + sideR), pipeR + sideR,anchor.z);
        edge_1.push_back(p);
        edge_2.push_back(glm::vec3(p.x*(-1), p.y, p.z));
    }
    reverse(edge_1.begin(), edge_1.end());
    edge_3.push_back(edge_2.back());
    edge_3.push_back(edge_1.front());
    return generatePlane(anchor, edgeVec);
}

BoundingBox Tee::boundingBox(){
    return box;
}

Tee::~Tee(){

}

vector<HalfPoint> Tee::intersectionPoints(glm::vec3 pos, glm::vec3 dir){
    vector<HalfPoint> result;
    vector<HalfPoint> ringPoints;
    vector<HalfPoint> cylinderPoints;
    vector<HalfPoint> tpPoints;
    for(auto r:ringVec){
        ringPoints = r.intersectionPoints(pos, dir);
        for(auto p:ringPoints){
            result.push_back(p);
        }
    }
    for(auto tp:triEdgePlaneVec){
        tpPoints = tp.intersectionPoints(pos, dir);
        for(auto p:tpPoints){
            result.push_back(p);
        }
    }
    for(auto c:pipeHalfVec){
        cylinderPoints = c.intersectionPoints(pos, dir);
        for(auto p:cylinderPoints){
            result.push_back(p);
        }
    }
    return result;
}

vector<HalfPoint> Tee::intersectionPointsInPipe(glm::vec3 pos, glm::vec3 dir){
    vector<HalfPoint> result;
    vector<HalfPoint> cylinderPoints;
    auto& pipeHalf = pipeHalfVec.at(0);
    QString exceptId = pipeHalf.getId();
    for(auto& c:cylinderVec){
        if(QString(c.getId())!=exceptId){
            cylinderPoints = c.intersectionPoints(pos, dir);
            for(auto p:cylinderPoints){
                result.push_back(p);
            }
        }
    }
    return result;
}


void Tee::setIdUsing(IdGenerator g){
    for(auto& i:ringVec){
        auto s = g.getRingId();
        i.setId(s.toLatin1().data());
    }
    for(auto& i:cylinderVec){
        i.setId(g.getCylinderId().toLatin1().data());
    }
    for(auto& i:planeVec){
        //i.setId(g.getPlaneId().toLatin1().data());
    }
    for(auto& i:triEdgePlaneVec){
        i.setId(g.getTriEdgePlaneId().toLatin1().data());
    }
}

Ring* Tee::getRing(QString id){
    for(auto& i:ringVec){
        if(id == QString(i.getId()) ){
            return &i;
        }
    }
    return nullptr;
}

TriEdgePlane* Tee::getTriPlane(QString id){
    for(auto &i:triEdgePlaneVec){
        if(id == QString(i.getId())){
            return &i;
        }
    }
    return nullptr;
}

Cylinder* Tee::getCylinder(QString id){
    for(auto &i:cylinderVec){
        if(id == QString(i.getId())){
            return &i;
        }
    }
    return nullptr;
}

std::map<QString, QString> Tee::edgeTopo(){
    vector<EdgePtr> edges;
    for(auto &i:pipeHalfVec){
        for(auto &ii:i.getEdges()) {
            edges.push_back(ii);
        }
    }

    for(auto &i:ringVec){
        for(auto &ii:i.getEdges()) {
            edges.push_back(ii);
        }
    }
    for(auto &i:triEdgePlaneVec){
        for(auto &ii:i.getEdges()) {
            edges.push_back(ii);
        }
    }
    std::map<QString, QString> topo;
    for(auto& i:edges){
        topo[i->Id()] = QString("");
    }
    for(auto& i:edges){
        for(auto &ii:edges){
            if(glm::length(i->center() - ii->center()) < 1){
                if(i->Id() != ii->Id()){
                    topo[i->Id()] = ii->Id();
                    topo[ii->Id()] = i->Id();
                }
            }
        }
    }
    return topo;
}

QString Tee::topoValue(QString key){
    auto topo = edgeTopo();
    if(topo.find(key) == topo.end()){
        assert(0);
    }
    return topo[key];
}

QString Tee::allTopoValue(QString key){
    auto topo = allEdgeTopo();
    if(topo.find(key) == topo.end()){
        assert(0);
    }
    return topo[key];
}

std::map<QString, QString> Tee::allEdgeTopo(){
    vector<EdgePtr> edges;
    for(auto &i:cylinderVec){
        for(auto &ii:i.getEdges()) {
            edges.push_back(ii);
        }
    }

    for(auto &i:ringVec){
        for(auto &ii:i.getEdges()) {
            edges.push_back(ii);
        }
    }
    for(auto &i:triEdgePlaneVec){
        for(auto &ii:i.getEdges()) {
            edges.push_back(ii);
        }
    }
    std::map<QString, QString> topo;
    for(auto& i:edges){
        topo[i->Id()] = QString("");
    }
    for(auto& i:edges){
        for(auto &ii:edges){
            if(glm::length(i->center() - ii->center()) < 1){
                if(i->Id() != ii->Id()){
                    topo[i->Id()] = ii->Id();
                    topo[ii->Id()] = i->Id();
                }
            }
        }
    }
    return topo;
}

Dir Tee::outNorm(Pos p, QString meshId){
    if(meshId.contains("ring")){
        Ring* r = getRing(meshId);
        RingAssist ass(*r);
        return ass.outNorm(p);
    }
    else if(meshId.contains("cylinder")){
        Cylinder* cy = getCylinder(meshId);
        CylinderAssist ass(*cy);
        return ass.outNorm(p);
    }
    else if(meshId.contains("plane")){
        TriEdgePlane* tri =  getTriPlane(meshId);
        TriEdgePlaneAssist ass(*tri);
        return ass.outNorm(p);
    }
    else{
        assert(0);
    }
}

QString Tee::symmmetryMesh(QString meshId , QString flag){
    QString sMesh = meshId;
    for(int i = 0;i < flag.size(); i++){
        QString s = flag.at(i);
        if(s.contains("x", Qt::CaseInsensitive)){
            sMesh = symMap.at(pair<QString, QString>(sMesh, "x"));
        }
        else if(s.contains("z", Qt::CaseInsensitive)){
            sMesh = symMap.at(pair<QString, QString>(sMesh, "z"));
        }
        else{
            assert(0);
        }
    }
    return sMesh;
}

QStringVec  Tee::getLeftCylinderId(){
    return leftCylinderId;
}

QStringVec Tee::getUpCylinderId(){
    return upCylinderId;
}

QStringVec Tee::getRightCylinderId(){
    return rightCylinderId;
}

glm::mat4 Tee::leftMat4(){
    glm::mat4 T;
    utility::setPos(T, Pos{(pipeR+sideR)*-1, 0, 0});
    utility::setXDir(T, Dir{0,0,1});
    utility::setYDir(T, Dir{0,1,0});
    utility::setZDir(T, Dir{-1, 0, 0});
    return T;
}

glm::mat4 Tee::upMat4(){
    glm::mat4 T;
    utility::setPos(T, Pos{0, (pipeR+sideR), 0});
    utility::setXDir(T, Dir{-1,0,0});
    utility::setYDir(T, Dir{0,0,1});
    utility::setZDir(T, Dir{0, 1, 0});
    return T;
}

glm::mat4 Tee::rightMat4(){
    glm::mat4 T;
    utility::setPos(T, Pos{(pipeR+sideR), 0, 0});
    utility::setXDir(T, Dir{0,0,-1});
    utility::setYDir(T, Dir{0,1,0});
    utility::setZDir(T, Dir{1, 0, 0});
    return T;
}


TeePara Tee::teePara(){
    return TeePara{pipeR, sideR, lengthMain, lengthBranch};
}


QString Tee::whichPart(QString meshId){
    auto leftIds = getLeftCylinderId();
    for(auto id:leftIds){
        if(id == meshId){
            return "left";
        }
    }
    auto upIds = getUpCylinderId();
    for(auto id:upIds){
        if(id == meshId){
            return "up";
        }
    }
    auto rightIds = getRightCylinderId();
    for(auto id:rightIds){
        if(id == meshId){
            return "right";
        }
    }
    return "";
}

QString Tee::cBand2TBandMesh(QString cBandMesh){
    QString which = whichPart(cBandMesh);
    Cylinder* cylinder = getCylinder(cBandMesh);
    auto edges = cylinder->getEdges();
    QString tEdgeId;
    EdgePtr edge;
    for(auto& e:edges){
        if(which == "up"){
            if(abs(e->center().y - (pipeR+sideR))<0.1){
                edge = e;
                break;
            }
        }
        else if(which == "right"){
            if(abs(e->center().x - (pipeR+sideR)) < 0.1){
                edge = e;
                break;
            }
        }
        else if(which == "left"){
            if(abs(e->center().x + (pipeR + sideR)) < 0.1){
                edge = e;
                break;
            }
        }
        else{
            assert(0);
        }
    }
    tEdgeId = allTopoValue(edge->Id());
    QString tMeshId = tEdgeId.split("_").at(0);
    return tMeshId;
}

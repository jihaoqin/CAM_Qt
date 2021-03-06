#include "Controller.h"
#include <memory>
#include "glwidget.h"
#include "TabWidget.h"
#include "mainwindow.h"
#include "GuiConnector.h"
#include "Cylinder.h"
#include "Ring.h"
#include "Point.h"
#include "Data.h"
#include "Node.h"
#include "NewCurveTab.h"
#include "RingCurveAssist.h"
#include "Curve.h"
#include "RingCurve.h"
#include "TriEdgePlaneCurve.h"
#include "TriEdgePlane.h"
#include "Cylinder.h"
#include "CylinderCurve.h"
#include "TCurve.h"
#include "TeeCurve.h"
#include "TBandOnPoint.h"
#include "PointSym.h"
#include <fstream>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "LeftCylinderAssist.h"
#include "Band.h"
#include "GeneralBand.h"
#include "GenCurveProgressDialog.h"
#include "OpenBandThread.h"
#include "FromPipeCurveTab.h"
#include "CBandOnPoint.h"

using namespace  std;
using namespace  rapidjson;
Controller::Controller()
{

}



void Controller::draw(){
    QMutexLocker locker(&data->mtx);
    NodePtr root = data->root;
    if(root != nullptr){
        auto drawFunc = [=](DataObjectPtr p)->void{ if(p != nullptr){drawDataObject(p);}};
        root->dataOperation(drawFunc);
    }
}

std::shared_ptr<Camera2> Controller::getCamera(){
    return data->camera;
}
void Controller::bindData(std::shared_ptr<Data> d){
    data = d;
}

BoundingBox Controller::updateBoundingBox(){
    data->updateBoundingBox();
    return data->box;
}

void Controller::addTee(float mainLength, float branchLength, float R, float sideR){
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR, data->idGenerator);
    QOpenGLContext* c = widget->getGLContext();
    t->bindGL(c);
    data->addTee(t);
    t->setIdUsing(data->idGenerator);
    mainWindow->updateAction();
}


std::shared_ptr<Point> Controller::addPoint(HalfPoint p){
    QString id = data->idGenerator.getPointId();
    std::shared_ptr<Point> point = std::make_shared<Point>(p.pos, id.toLatin1().data());
    point->meshId(p.meshName.c_str());
    QOpenGLContext* c = widget->getGLContext();
    point->bindGL(c);
    data->addPoint(point);
    mainWindow->updateAction();
    return point;
}


bool Controller::hasTee(){
    return data->hasTee();
}

void Controller::setViewPortRatio(int w, int h){
    data->setViewPortRatio(w, h);
}

void Controller::processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    data->processTranslation(mPos, mLastPos, viewPort);
}

void Controller::processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    data->processRotation(mPos, mLastPos, viewPort);
}

void Controller::processScroll(double yOffset){
    data->processScroll(yOffset);
}


void Controller::clearData(){
    data->clear();
}

bool Controller::getEmpty(){
    return data->getEmpty();
}

bool Controller::getChanged(){
    return data->getChanged();
}

void Controller::bindGLWidget(GLWidget* w){
    widget = w;
}

void Controller::bindMainWindow(MainWindow* m){
    mainWindow = m;
}

void Controller::processIntersectionPoint(glm::vec3 begin, glm::vec3 dir, glm::vec2 glXY){
    //判断是创建点，还是选择点
        //根据pointText内是否已有可视点，如果有可视点，就是选择点操作，如果无可视点就是创建点操作
    //如果是选择点操作
        //后面的点击操作只改变此点的位置，并不新创建点
    //如果是创建点操作
        //创建点
    TabWidget* tabWidget = mainWindow->connector->getTabWidget();
    QString job = tabWidget->doWhat();
    if(job == QString("newCurveTab")){
        NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
        QString pointId = newCurveTab->getPointText();
        bool focusOnPointText = newCurveTab->isPointTextFocused();
        if(focusOnPointText == true){
            if(pointId.isEmpty()){
                //创建点
                QString id = addIntersectionPointInTee(begin, dir);
                newCurveTab->setPointText(id);
                float angle = newCurveTab->getWindingAngle();
                QString curveId = addBandInTee(id,angle);
                newCurveTab->setCurveId(curveId);
            }
            else{
                //赋选中点picked
                clickOnPoint(pointId, glXY);
            }
        }
        else{
            return ;
        }
    }
    else if(job == QString("fromPipeCurveTab")){
        FromPipeCurveTab* pipeTab = mainWindow->connector->getFromPipeCurveTab();
        QString pointId = pipeTab->getPointText();
        bool focusOnPointText = pipeTab->isPointTextFocused();
        if(focusOnPointText == true){
            if(pointId.isEmpty()){
                QString id = addIntersectionPointInPipe(begin, dir);
                if(!id.contains("point")){
                    return ;
                }
                pipeTab->setPointText(id);
                float angle = pipeTab->getWindingAngle();
                CBandPtr cBand = addCBandInPipe(id, angle);
                pipeTab->setCBandId(cBand->getId());
                TeePtr tee = data->root->findTeePtr();
                QString tMeshId = tee->cBand2TBandMesh(cBand->frontMeshId());
                EndPtr frontE = cBand->bandEnd()->frontEnd();
                PointPtr pointTee = make_shared<Point>(frontE->pd.pos, data->idGenerator.getPointId().toLatin1().data());
                pointTee->meshId(tMeshId.toLatin1().data());
                pipeTab->setTPointId(pointTee->getId());
                auto gl = widget->getGLContext();
                pointTee->bindGL(gl);
                data->addPoint(pointTee);
                if(tMeshId.contains("ring")){
                    Ring* ring = tee->getRing(tMeshId);
                    RingAssist rAssist(*ring);
                    CPPara para = rAssist.worldToCPPara(frontE->pd.pos, frontE->pd.dir);
                    QString curveId = addBandInTee(pointTee->getId(), para.uAng);
                    pipeTab->setTCurveId(curveId);
                }
                else if(tMeshId.contains("cylinder")){
                    Cylinder* cylinder = tee->getCylinder(tMeshId);
                    CylinderAssist cyAssist(*cylinder);
                    CPPara para = cyAssist.worldToCPPara(frontE->pd.pos, frontE->pd.dir);
                    QString curveId = addBandInTee(pointTee->getId(), para.uAng);
                    pipeTab->setTCurveId(curveId);
                }
                else{
                    assert(0);
                }
            }
            else{
                clickOnPoint(pointId, glXY);
            }
        }
        else{
            return;
        }
    }
    else{
        return;
    }
}

std::vector<HalfPoint> Controller::intersectionPointInTee(glm::vec3 begin, glm::vec3 dir){
    DataObjectPtr teeBase = data->root->findObjectId("tee");
    auto tee = dynamic_pointer_cast<Tee>(teeBase);
    if(tee == nullptr){
        return vector<HalfPoint>();
    }
    vector<HalfPoint> points = tee->intersectionPoints(begin, dir);
    if(points.size() == 0){
        return vector<HalfPoint>();
    }
    else{
        float dist = utility::length(points.at(0).pos - begin);
        int minIndex = 0;
        for(int i = 0; i < points.size(); i++){
            float l = utility::length(points.at(i).pos - begin);
            if(l < dist){
                dist = l;
                minIndex = i;
            }
        }
        return std::vector<HalfPoint>{points.at(minIndex)};
    }
}

std::vector<HalfPoint> Controller::intersectionPointInPipe(glm::vec3 begin, glm::vec3 dir){
    DataObjectPtr teeBase = data->root->findObjectId("tee");
    auto tee = dynamic_pointer_cast<Tee>(teeBase);
    if(tee == nullptr){
        return vector<HalfPoint>();
    }
    vector<HalfPoint> points = tee->intersectionPointsInPipe(begin, dir);
    if(points.size() == 0){
        return vector<HalfPoint>();
    }
    else{
        float dist = utility::length(points.at(0).pos - begin);
        int minIndex = 0;
        for(int i = 0; i < points.size(); i++){
            float l = utility::length(points.at(i).pos - begin);
            if(l < dist){
                dist = l;
                minIndex = i;
            }
        }
        return std::vector<HalfPoint>{points.at(minIndex)};
    }
}

QString Controller::addIntersectionPointInTee(glm::vec3 begin, glm::vec3 dir){
    auto pointVec = intersectionPointInTee(begin,dir);
    if(pointVec.size()==0){
        return QString();
    }
    else{
        auto pointPtr = addPoint(pointVec.at(0));
        return QString(pointPtr->getId());
    }
}

QString Controller::addIntersectionPointInPipe(glm::vec3 begin, glm::vec3 dir){
    auto pointVec = intersectionPointInPipe(begin,dir);
    if(pointVec.size()==0){
        return QString();
    }
    else{
        auto pointPtr = addPoint(pointVec.at(0));
        return QString(pointPtr->getId());
    }
}

void Controller::drawDataObject(std::shared_ptr<DataObject> ob){
    assert(ob);
    QString id(ob->getId());
    std::shared_ptr<GLProgram> p;
    if(id.contains("point")){
        p = mainWindow->connector->getPointProgram();
    }
    else{
        p = mainWindow->connector->getMeshProgram();
    }
    p->bind();
    data->camera->setUniform(p);
    ob->draw(p);
}


void Controller::clickOnPoint(QString id, glm::vec2 sample2D){
    auto root = data->root;
    if(root == nullptr){
        return ;
    }
    DataObjectPtr objectPtr = root->findObjectId(id.toLatin1().data());
    if(objectPtr == nullptr){
        return ;
    }
    auto pointPtr = dynamic_pointer_cast<Point>(objectPtr);
    glm::vec3 pos3D = pointPtr->getPos();
    GLWidget *glWidget = mainWindow->connector->getGLWidget();
    glm::vec2 pos2D = glWidget->spatialTo2D(pos3D);
    if(utility::length(sample2D - pos2D) < 4){
        pointPtr->picked = true;
    }
}

void Controller::processIntersectionWhenRelease(){
    auto tabWidget = mainWindow->connector->getTabWidget();
    QString job = tabWidget->doWhat();
    bool focusOnPointText;
    QString pointId;
    if(job == "newCurveTab"){
        NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
        focusOnPointText = newCurveTab->isPointTextFocused();
        pointId = newCurveTab->getPointText();
    }
    else if(job == "fromPipeCurveTab"){
        FromPipeCurveTab* pipeTab = mainWindow->connector->getFromPipeCurveTab();
        focusOnPointText = pipeTab->isPointTextFocused();
        pointId = pipeTab->getPointText();
    }
    else{
        return;
    }
    if(focusOnPointText){
        setPointPickState(pointId, false);
    }
    else{
        return;
    }
}

void Controller::exitPick(){
    processIntersectionWhenRelease();
}

void Controller::setPointPickState(QString id, bool state){
    if(id.isEmpty()){
        return;
    }
    DataObjectPtr ptr = data->root->findObjectId(id.toLatin1().data());
    if(ptr){
        auto pointPtr = dynamic_pointer_cast<Point>(ptr);
        pointPtr->picked = state;
    }
}

void Controller::processMoveWhenMove(Pos begin, Dir dir){
    auto tabWidget = mainWindow->connector->getTabWidget();
    QString job = tabWidget->doWhat();
    if(job == QString("newCurveTab")){
        processMoveInTee(begin, dir);
    }
    else if(job == QString("fromPipeCurveTab")){
        processMoveInPipe(begin, dir);
    }
    else{
        return;
    }
}

void Controller::processMoveInTee(glm::vec3 begin, glm::vec3 dir){
    NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
    bool focusOnPointText = newCurveTab->isPointTextFocused();
    if(focusOnPointText){
        QString pointId = newCurveTab->getPointText();
        DataObjectPtr ptr = data->root->findObjectId(pointId.toLatin1().data());
        if(ptr == nullptr){
            return;
        }
        else{
            auto pointPtr = dynamic_pointer_cast<Point>(ptr);
            if(pointPtr->picked == true){
                auto pointVec = intersectionPointInTee(begin,dir);
                if(pointVec.size()==0){
                    return;
                }
                glm::vec3 pos = pointVec.at(0).pos;
                pointPtr->setPos(pos);
                pointPtr->meshId(pointVec.at(0).meshName.c_str());
            }
        }
    }
    else{
        return;
    }
}

void Controller::processMoveInPipe(Pos begin, Dir dir){
    FromPipeCurveTab* pipeTab = mainWindow->connector->getFromPipeCurveTab();
    bool focusOnPointText = pipeTab->isPointTextFocused();
    if(focusOnPointText){
        QString pointId = pipeTab->getPointText();
        DataObjectPtr ptr = data->root->findObjectId(pointId.toLatin1().data());
        if(ptr == nullptr){
            return;
        }
        else{
            auto pointPtr = dynamic_pointer_cast<Point>(ptr);
            if(pointPtr->picked == true){
                auto pointVec = intersectionPointInPipe(begin,dir);
                if(pointVec.size()==0){
                    return;
                }
                glm::vec3 pos = pointVec.at(0).pos;
                pointPtr->setPos(pos);
                pointPtr->meshId(pointVec.at(0).meshName.c_str());
                pipeTab->updateBand();
            }
        }
    }
    else{
        return;
    }
}


CBandPtr Controller::addCBandInPipe(QString pId, float uAng){
    if(!pId.contains("point")){
        return nullptr;
    }
    auto ptr = data->root->findObjectId(pId.toLatin1().data());
    if(ptr == nullptr){
        return nullptr;
    }
    auto pointPtr = std::dynamic_pointer_cast<Point>(ptr);
    auto teeFa = data->root->findObjectId("tee");
    auto teePtr = std::dynamic_pointer_cast<Tee>(teeFa);
    QString meshId(pointPtr->meshId());
    QString bandId = data->idGenerator.getBandId();
    auto band = std::make_shared<CBandOnPoint>(HalfPoint{pointPtr->getPos(), meshId.toLatin1().data()},
                                               uAng, teePtr, bandId, data->bandWidth());
    QOpenGLContext* gl = widget->getGLContext();
    band->bindGL(gl);
    data->addBand(band);
    mainWindow->updateModel();
    mainWindow->updateAction();
    return band;
}

QString Controller::addBandInTee(QString pId, float uAng){
    if(!pId.contains("point")){
        return QString("");
    }
    auto ptr = data->root->findObjectId(pId.toLatin1().data());
    if(ptr == nullptr){
        return QString("");
    }
    auto pointPtr = std::dynamic_pointer_cast<Point>(ptr);
    auto teeFa = data->root->findObjectId("tee");
    auto teePtr = std::dynamic_pointer_cast<Tee>(teeFa);
    QString meshId(pointPtr->meshId());
    QString id = data->idGenerator.getCurveId();
    auto curve = std::make_shared<TCurve>(pointPtr, uAng, 0, id.toLatin1().data(), teePtr);
    pointPtr->addChild(curve);
    QString bandId = data->idGenerator.getBandId();
    auto band = std::make_shared<TBandOnPoint>(pointPtr, data->bandWidth(), curve, bandId, teePtr);
    pointPtr->addChild(band);
    curve->addChild(band);
    QOpenGLContext* gl = widget->getGLContext();
    curve->bindGL(gl);
    band->bindGL(gl);
    data->addBand(band);
    data->addCurve(curve);
    mainWindow->updateModel();
    mainWindow->updateAction();
    return id;
    /*
    if(meshId.contains("ring")){
        QString id = data->idGenerator.getCurveId();
        Ring* r = teePtr->getRing(meshId);
        auto curve = std::make_shared<RingCurve>(pointPtr,uAng,0.1,id.toLatin1().data(), r);
        pointPtr->addChild(curve);
        QOpenGLContext* gl = widget->getGLContext();
        curve->bindGL(gl);
        data->addCurve(curve);
        mainWindow->updateAction();
        return id;
    }
    else if(meshId.contains("plane")){
        TriEdgePlane* tri = teePtr->getTriPlane(meshId);
        assert(tri);
        QString id = data->idGenerator.getCurveId();
        auto curve = std::make_shared<TriEdgePlaneCurve>(pointPtr, uAng, 0.1, id.toLatin1().data(),  tri);
        pointPtr->addChild(curve);
        QOpenGLContext* gl = widget->getGLContext();
        curve->bindGL(gl);
        data->addCurve(curve);
        mainWindow->updateAction();
        return id;
    }
    else if(meshId.contains("cylinder")){
        Cylinder* cy = teePtr->getCylinder(meshId);
        assert(cy);
        QString id = data->idGenerator.getCurveId();
        auto curve = std::make_shared<CylinderCurve>(pointPtr, uAng, 0.1, id.toLatin1().data(), cy);
        pointPtr->addChild(curve);
        QOpenGLContext* gl = widget->getGLContext();
        curve->bindGL(gl);
        data->addCurve(curve);
        mainWindow->updateAction();
        return id;
    }
    else{
        return QString();
    }
    */
}

void Controller::updateCurve(QString id, float angle, float coe){
    DataObjectPtr ptr = data->root->findObjectId(id.toLatin1().data());
    assert(ptr);
    QString ptrId = QString(ptr->getId());
    if(!ptrId.contains("curve")){
        return;
    }
    auto curve = std::dynamic_pointer_cast<Curve>(ptr);
    curve->setWindingPara(angle, coe);
}

void Controller::deleteBand(QString id){
    data->deleteBand(id);
}

void Controller::setColor(QStringVec ids, Color c){
    for(auto s:ids){
        auto obj = data->root->findObjectId(s.toLatin1().data());
        if(obj){
            if(s.contains("band")){
                auto band = std::dynamic_pointer_cast<Band>(obj);
                band->setColor(c);
            }
            else{
                assert(0);
            }
        }
    }
}

void Controller::saveBand(QString path){
    auto root = data->root;
    auto children = root->childrenPtrVec();
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    writer.StartArray();
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr b = std::dynamic_pointer_cast<Band>(c->getData());
            if(b->typeStr() == "TBandOnPoint"){
                b->serialize(writer);
            }
            else if(b->typeStr() == "GeneralBand"){
                b->serialize(writer);
            }
        }
    }
    writer.EndArray();
    std::ofstream outFile;
    outFile.open(path.toLatin1().data());
    outFile<<sb.GetString();
    outFile.close();
}

void Controller::openBand(QString path){
    /*
    std::ifstream inFile;
    string line;
    string allLine;
    inFile.open(path.toLatin1().data());
    while(getline(inFile, line)){
        allLine.append(line+"\n");
    }
    Document doc;
    doc.Parse<0>(allLine.c_str());
    if(doc.HasParseError()){
        return ;
    }
    for(unsigned int i = 0; i < doc.Size(); i++){
        const Value& obj = doc[i];
        if(obj["type"] == "TBandOnPoint"){
            shared_ptr<TBandOnPoint> band;
            shared_ptr<Curve> curvePtr;
            shared_ptr<Point> pointPtr;
            float width = obj["width"].GetDouble();
            const Value& curve = obj["curve"];
            TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
            if(curve["type"] == "TCurve"){
                float uAng = curve["uAng"].GetDouble();
                float slip = curve["slip"].GetDouble();
                const Value& point = curve["point"];
                const Value& p = point["pos"];
                Pos pos{p[0].GetDouble(), p[1].GetDouble(), p[2].GetDouble()};
                QString meshId = QString(point["meshId"].GetString());
                pointPtr = make_shared<Point>(pos, data->idGenerator.getPlaneId().toLatin1().data());
                pointPtr->meshId(meshId.toLatin1().data());
                curvePtr = make_shared<TCurve>(pointPtr, uAng, slip, data->idGenerator.getCurveId().toLatin1().data(), tee);
                shared_ptr<TCurve> tPtr = dynamic_pointer_cast<TCurve>(curvePtr);
                band = make_shared<TBandOnPoint>(pointPtr, width, tPtr, data->idGenerator.getBandId(), tee);
                pointPtr->setVisiable(false);
                curvePtr->setVisiable(false);
                band->setColor(Color::YELLOW);
                data->addPoint(pointPtr);
                data->addCurve(curvePtr);
                data->addBand(band);
                auto context = widget->getGLContext();
                pointPtr->bindGL(context);
                curvePtr->bindGL(context);
                band->bindGL(context);
                pointPtr->addChild(curvePtr);
                curvePtr->addChild(band);
            }
        }
    }
    inFile.close();
    */
}

void Controller::genLeftCurve(){
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    genCylinderCurve("left");
    /*
    LeftCylinderAssist leftAssist(tee, tee->getLeftCylinderId());
    auto root = data->root;
    auto children = root->childrenPtrVec();
    PosDirVec beginSide;
    PosDirVec endSide;
    QStringVec beginMesh;
    QStringVec endMesh;
    vector<EndPtr> endVec;
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr b = std::dynamic_pointer_cast<Band>(c->getData());
            BandEndPtr backend = b->bandEnd();
            bool flag1 = leftAssist.isPosIn(backend->frontPos());
            if(flag1){
                endVec.push_back(backend->frontEnd());
            }
            bool flag2 = leftAssist.isPosIn(backend->backPos());
            if(flag2){
                endVec.push_back(backend->backEnd());
            }
        }
    }
    for(auto& e:endVec){
        if(!e->nextEndId.isEmpty()){
            continue;
        }
        auto tuple1 = leftAssist.genCurve(e);
        auto& pds = get<0>(tuple1);
        auto& strs = get<1>(tuple1);
        GeneralBandPtr band = make_shared<GeneralBand>(pds, strs, data->idGenerator.getBandId(), tee);
        QOpenGLContext* gl = widget->getGLContext();
        band->setCouple(e);
        band->bindGL(gl);
        data->addBand(band);
    }
    //找环
    EndPtrVec couplingEndVec;
    children = data->root->childrenPtrVec();
    for(auto c:children){
        DataObjectPtr objPtr = c->getData();
        if(QString(objPtr->getId()).contains("band")){
            BandPtr band = dynamic_pointer_cast<Band>(objPtr);
            BandEndPtr bandEnd = band->bandEnd();
            for(auto end:bandEnd->ends){
                if(leftAssist.isReturn(end)){
                    couplingEndVec.push_back(end);
                }
            }
        }
    }
    for(auto e:couplingEndVec){
        if(e->isCoupled()){
            continue;
        }
        auto dirEndVec = leftAssist.filterDir(e, couplingEndVec);
        auto cycleEndVec = leftAssist.filterCycle(e, dirEndVec, allEnds());
        auto innerFirstEndVec = leftAssist.filterInnerFirst(cycleEndVec, allEnds());
        auto nearEnd = leftAssist.nearEnd(e, innerFirstEndVec);
        auto tuple1 = leftAssist.genCircleCurve(e, nearEnd);
        auto& pds = get<0>(tuple1);
        auto& strs = get<1>(tuple1);
        auto band = make_shared<GeneralBand>(pds, strs, data->idGenerator.getBandId(), tee);
        QOpenGLContext* gl = widget->getGLContext();
        band->setCouple(e);
        band->setCouple(nearEnd);
        band->bindGL(gl);
        data->addBand(band);
    }
    */
}

EndPtrVec Controller::allEnds(){
    EndPtrVec all;
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    auto root = data->root;
    auto children = root->childrenPtrVec();
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr b = std::dynamic_pointer_cast<Band>(c->getData());
            BandEndPtr backend = b->bandEnd();
            all.push_back(backend->frontEnd());
            all.push_back(backend->backEnd());
        }
    }
    return all;
}

std::vector<End> Controller::allEndsVec(){
    EndPtrVec endPtrs = allEnds();
    vector<End> endsVec;
    for(auto e:endPtrs){
        endsVec.push_back(*e);
    }
    return endsVec;
}

std::set<End> Controller::allEndsSet(){
    EndPtrVec endPtrs = allEnds();
    set<End> endsSet;
    for(auto e:endPtrs){
        endsSet.insert(*e);
    }
    return endsSet;
}

void Controller::genCylinderCurve(QString which){
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    LeftCylinderAssist leftAssist(tee, which);
    auto root = data->root;
    auto children = root->childrenPtrVec();
    PosDirVec beginSide;
    PosDirVec endSide;
    QStringVec beginMesh;
    QStringVec endMesh;
    vector<EndPtr> endVec;
    for(auto c:children){
        QString id = c->Id();
        if(id.contains("band")){
            BandPtr b = std::dynamic_pointer_cast<Band>(c->getData());
            BandEndPtr backend = b->bandEnd();
            bool flag1 = leftAssist.isPosAtStart(backend->frontPos())&&backend->frontEnd()->nextEndId.isEmpty();
            if(flag1){
                endVec.push_back(backend->frontEnd());
            }
            bool flag2 = leftAssist.isPosAtStart(backend->backPos())&&backend->backEnd()->nextEndId.isEmpty();
            if(flag2){
                endVec.push_back(backend->backEnd());
            }
        }
    }
    for(auto& e:endVec){
        auto tuple1 = leftAssist.genCurve(e);
        auto& pds = get<0>(tuple1);
        auto& strs = get<1>(tuple1);
        GeneralBandPtr band = make_shared<GeneralBand>(pds, strs, data->idGenerator.getBandId(), tee, data->bandWidth());
        QOpenGLContext* gl = widget->getGLContext();
        band->setCouple(e);
        band->bindGL(gl);
        data->addBand(band);
    }
}

void Controller::genUpCurve(){
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    genCylinderCurve("up");
}

void Controller::genRightCurve(){
    TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
    genCylinderCurve("right");
}


void Controller::allBindGL(){
    auto children = data->root->childrenPtrVec();
    auto gl = widget->getGLContext();
    for(auto c:children){
        auto obj = c->getData();
        obj->bindGL(gl);
    }
}


std::shared_ptr<Data> Controller::getData(){
    return data;
}


QOpenGLContext* Controller::getGLContext(){
    return widget->getGLContext();
}

GuiConnector* Controller::getConnector(){
    return mainWindow->connector;
}

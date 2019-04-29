#include "OpenBandThread.h"
#include "Controller.h"
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "TBandOnPoint.h"
#include "Data.h"
#include "Controller.h"
#include "Node.h"
#include "glwidget.h"
#include "GeneralBand.h"
#include <map>
using namespace std;

OpenBandThread::OpenBandThread(QString p, Controller* c)
    :path(p), ctrl(c)
{

}

void OpenBandThread::run(){
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
    auto& mtx = ctrl->data->mtx;
    std::shared_ptr<Data> data = ctrl->data;
    int sum = 0;
    for(unsigned int i = 0; i < doc.Size(); i++){
        const Value& obj = doc[i];
        if(obj["type"] == "TBandOnPoint" || obj["type"] == "GeneralBand"){
            ++sum;
        }
    }
    int left = sum;
    emit progress(sum, left);
    std::map<QString, QString> fileEnd2NewEnd;
    std::map<QString, QString> newEnd2FileEnd;
    map<QString, QString> fileEndNextEnd;
    for(unsigned int i = 0; i < doc.Size(); i++){
        const Value& obj = doc[i];
        if(obj["type"] == "TBandOnPoint"){
            shared_ptr<TBandOnPoint> band;
            QString newBandId = data->idGenerator.getBandId();
            shared_ptr<Curve> curvePtr;
            shared_ptr<Point> pointPtr;
            float width = obj["width"].GetDouble();
            const Value& curve = obj["curve"];
            const Value& ends = obj["ends"];
            {
                const Value& frontEnd = ends["frontEnd"];
                const Value& backEnd = ends["backEnd"];
                QString frontEndId = frontEnd["endId"].GetString();
                QString backEndId = backEnd["endId"].GetString();
                QString frontNextId = frontEnd["nextEndId"].GetString();
                QString backNextId = backEnd["nextEndId"].GetString();
                fileEndNextEnd[frontEndId] = frontNextId;
                fileEndNextEnd[backEndId] = backNextId;
                fileEnd2NewEnd[frontEndId] = newBandId+".front";
                fileEnd2NewEnd[backEndId] = newBandId+".back";
                newEnd2FileEnd[newBandId+".front"] = frontEndId;
                newEnd2FileEnd[newBandId+".back"] = backEndId;
            };
            TeePtr tee = dynamic_pointer_cast<Tee>(data->root->findObjectId("tee"));
            if(curve["type"] == "TCurve"){
                float uAng = curve["uAng"].GetDouble();
                float slip = curve["slip"].GetDouble();
                const Value& point = curve["point"];
                const Value& p = point["pos"];
                Pos pos{p[0].GetDouble(), p[1].GetDouble(), p[2].GetDouble()};
                QString meshId = QString(point["meshId"].GetString());
                mtx.lock();
                pointPtr = make_shared<Point>(pos, data->idGenerator.getPointId().toLatin1().data());
                pointPtr->meshId(meshId.toLatin1().data());
                curvePtr = make_shared<TCurve>(pointPtr, uAng, slip, data->idGenerator.getCurveId().toLatin1().data(), tee);
                shared_ptr<TCurve> tPtr = dynamic_pointer_cast<TCurve>(curvePtr);
                band = make_shared<TBandOnPoint>(pointPtr, width, tPtr, newBandId, tee);
                pointPtr->setVisiable(false);
                curvePtr->setVisiable(false);
                band->setColor(Color::YELLOW);
                data->addPoint(pointPtr);
                data->addCurve(curvePtr);
                data->addBand(band);
                pointPtr->addChild(curvePtr);
                curvePtr->addChild(band);
                mtx.unlock();
                --left;
                emit progress(sum, left);
            }
        }
        else if(obj["type"] == "GeneralBand"){
            shared_ptr<GeneralBand> band;
            QString newBandId = data->idGenerator.getBandId();
            float width = obj["width"].GetDouble();
            const Value& ends = obj["ends"];{
                const Value& frontEnd = ends["frontEnd"];
                const Value& backEnd = ends["backEnd"];
                QString frontEndId = frontEnd["endId"].GetString();
                QString backEndId = backEnd["endId"].GetString();
                QString frontNextId = frontEnd["nextEndId"].GetString();
                QString backNextId = backEnd["nextEndId"].GetString();
                fileEndNextEnd[frontEndId] = frontNextId;
                fileEndNextEnd[backEndId] = backNextId;
                fileEnd2NewEnd[frontEndId] = newBandId+".front";
                fileEnd2NewEnd[backEndId] = newBandId+".back";
                newEnd2FileEnd[newBandId+".front"] = frontEndId;
                newEnd2FileEnd[newBandId+".back"] = backEndId;
            }
            const Value& poss = obj["poss"];
            const Value& dirs = obj["dirs"];
            PosDirVec pds;
            {
                for(int i = 0; i<poss.Size(); i = i+3){
                    float posx = poss[i].GetDouble();
                    float posy = poss[i+1].GetDouble();
                    float posz = poss[i+2].GetDouble();

                    float dirx = dirs[i].GetDouble();
                    float diry = dirs[i+1].GetDouble();
                    float dirz = dirs[i+2].GetDouble();
                    pds.push_back(PosDir{Pos{posx, posy, posz}, Dir{dirx, diry, dirz}});
                }
            }
            const Value& meshNames = obj["meshNames"];
            QStringVec strs;
            {
                for(int i = 0; i<meshNames.Size(); i++){
                    strs.push_back(meshNames[i].GetString());
                }
            }
            TeePtr tee = data->getNodeRoot()->findTeePtr();
            mtx.lock();
            band = make_shared<GeneralBand>(pds, strs, newBandId, tee, width);
            data->addBand(band);
            mtx.unlock();
            --left;
            emit progress(sum, left);
        }
    }
    fileEndNextEnd[""] = "";
    fileEnd2NewEnd[""] = "";
    auto childrenPtrs = data->getNodeRoot()->childrenPtrVec();
    for(auto& c:childrenPtrs){
        DataObjectPtr obj = c->getData();
        QString objId = obj->getId();
        if(newEnd2FileEnd.find(objId+".front") != newEnd2FileEnd.end()){
            auto tBandPtr = dynamic_pointer_cast<Band>(obj);
            QString newFrontEndId = QString(tBandPtr->getId())+".front";
            QString newBackEndId = QString(tBandPtr->getId())+".back";
            QString fileFrontEndId = newEnd2FileEnd.at(newFrontEndId);
            QString fileBackEndId = newEnd2FileEnd.at(newBackEndId);
            QString fileFrontNextEndId = fileEndNextEnd.at(fileFrontEndId);
            QString fileBackNextEndId = fileEndNextEnd.at(fileBackEndId);
            QString newFrontNextEndId = fileEnd2NewEnd.at(fileFrontNextEndId);
            QString newBackNextEndId = fileEnd2NewEnd.at(fileBackNextEndId);
            tBandPtr->bandEnd()->frontEnd()->nextEndId = newFrontNextEndId;
            tBandPtr->bandEnd()->backEnd()->nextEndId = newBackNextEndId;
        }
    }
    inFile.close();
    emit calOver();
}

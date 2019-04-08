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
    std::shared_ptr<Data> data = ctrl->data;
    int sum = 0;
    for(unsigned int i = 0; i < doc.Size(); i++){
        const Value& obj = doc[i];
        if(obj["type"] == "TBandOnPoint"){
            ++sum;
        }
    }
    int left = sum;
    emit(progress(sum, left));
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
                //auto context = ctrl->widget->getGLContext();
                //pointPtr->bindGL(context);
                //curvePtr->bindGL(context);
                //band->bindGL(context);
                pointPtr->addChild(curvePtr);
                curvePtr->addChild(band);
                --left;
                emit(progress(sum, left));
            }
        }
    }
    inFile.close();
    emit(calOver());
}

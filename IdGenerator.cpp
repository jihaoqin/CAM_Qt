#include "IdGenerator.h"

IdGenerator::IdGenerator()
    :cylinderIndex(0),ringIndex(0),pointIndex(0),planeIndex(0),curveIndex(0), triEdgePlaneIndex(0)
    ,bandIndex(0)
{
}

QString IdGenerator::getCylinderId(){
    QString id = QString("cylinder") + QString::number(cylinderIndex);
    cylinderIndex++;
    return id;
}

QString IdGenerator::getRingId(){
    QString id = QString("ring") + QString::number(ringIndex);
    ringIndex++;
    return id;
}

QString IdGenerator::getPointId(){
    QString id = QString("point") + QString::number(pointIndex);
    pointIndex++;
    return id;
}

QString IdGenerator::getPlaneId(){
    QString id = QString("plane") + QString::number(planeIndex);
    planeIndex++;
    return id;
}

QString IdGenerator::getCurveId(){
    QString id = QString("curve") + QString::number(curveIndex);
    curveIndex++;
    return id;
}

QString IdGenerator::getTriEdgePlaneId(){
    QString id = QString("triEdgePlane") + QString::number(triEdgePlaneIndex);
    triEdgePlaneIndex++;
    return id;
}

QString IdGenerator::getBandId(){
    QString id = QString("band")+QString::number(bandIndex);
    bandIndex++;
    return id;
}


void IdGenerator::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
    writer.StartObject();
    writer.String("type");
    writer.String("IdGenerator");
    writer.String("cylinderIndex");
    writer.Int(cylinderIndex);
    writer.String("ringIndex");
    writer.Int(ringIndex);
    writer.String("pointIndex");
    writer.Int(pointIndex);
    writer.String("planeIndex");
    writer.Int(planeIndex);
    writer.String("curveIndex");
    writer.Int(curveIndex);
    writer.String("triEdgePlaneIndex");
    writer.Int(triEdgePlaneIndex);
    writer.String("bandIndex");
    writer.Int(bandIndex);
    writer.EndObject();
}

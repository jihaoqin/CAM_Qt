#include "IdGenerator.h"

IdGenerator::IdGenerator():cylinderIndex(0),ringIndex(0),pointIndex(0),planeIndex(0),curveIndex(0), triEdgePlaneIndex(0)
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

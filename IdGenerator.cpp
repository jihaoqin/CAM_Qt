#include "IdGenerator.h"

IdGenerator::IdGenerator():cylinderIndex(0),ringIndex(0),pointIndex(0)
{

}

QString IdGenerator::getCylinderId(){
    QString id = QString("cylinder_") + QString::number(cylinderIndex);
    cylinderIndex++;
    return id;
}

QString IdGenerator::getRingId(){
    QString id = QString("ring_") + QString::number(ringIndex);
    ringIndex++;
    return id;
}

QString IdGenerator::getPointId(){
    QString id = QString("point_") + QString::number(pointIndex);
    pointIndex++;
    return id;
}

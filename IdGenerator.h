#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <QString>
#include "rapidjson/prettywriter.h"

class IdGenerator
{
public:
    IdGenerator();
    QString getCylinderId();
    QString getRingId();
    QString getPointId();
    QString getPlaneId();
    QString getCurveId();
    QString getTriEdgePlaneId();
    QString getBandId();
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
private:
    unsigned int cylinderIndex;
    unsigned int ringIndex;
    unsigned int pointIndex;
    unsigned int planeIndex;
    unsigned int curveIndex;
    unsigned int triEdgePlaneIndex;
    unsigned int bandIndex;

};

#endif // IDGENERATOR_H

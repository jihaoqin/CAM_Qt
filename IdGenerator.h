#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <QString>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


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
private:
    unsigned int cylinderIndex;
    unsigned int ringIndex;
    unsigned int pointIndex;
    unsigned int planeIndex;
    unsigned int curveIndex;
    unsigned int triEdgePlaneIndex;

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar& cylinderIndex & ringIndex & pointIndex & planeIndex;
    }
};

#endif // IDGENERATOR_H

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
private:
    unsigned int cylinderIndex;
    unsigned int ringIndex;
    unsigned int pointIndex;

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar& cylinderIndex & ringIndex & pointIndex;
    }
};

#endif // IDGENERATOR_H

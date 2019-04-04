#ifndef BAND_H
#define BAND_H

#include "DataObject.h"
#include "PosDir.h"
#include "Curve.h"

class Tee;
class End;
using Pos = glm::vec3;
using QStringVec = std::vector<QString>;
using TeePtr = std::shared_ptr<Tee>;
using EndPtr = std::shared_ptr<End>;
class End{
public:
    End(PosDir, QString, QString);
    QString bandId();
    bool isClose(EndPtr&);
    bool setCouple(EndPtr&);
    PosDir pd;
    QString nextEndId;
    QString endId;
};
using EndPtrVec = std::vector<EndPtr>;

class BandEnd{
public:
    BandEnd(QString, EndPtrVec);
    QString bandId;
    EndPtrVec ends;
    void setCouple(EndPtr&);
    EndPtr frontEnd();
    Pos frontPos();
    EndPtr backEnd();
    Pos backPos();
};
using BandEndPtr = std::shared_ptr<BandEnd>;
using BandEndPtrVec = std::vector<BandEndPtr>;

class Band:public DataObject
{
public:
    Band();
    virtual void setColor(Color);
    BandEndPtr bandEnd();
    void setCouple(EndPtr&);
protected:
    BandEndPtr m_end;
};

using BandPtr = std::shared_ptr<Band>;

#endif // BAND_H

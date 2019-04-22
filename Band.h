#ifndef BAND_H
#define BAND_H

#include "DataObject.h"
#include "PosDir.h"
#include "Curve.h"
#include "NameDeclare.h"
#include "Mesh.h"

class Tee;
class End;
using Pos = glm::vec3;
using QStringVec = std::vector<QString>;
using TeePtr = std::shared_ptr<Tee>;
using EndPtr = std::shared_ptr<End>;
class End{
public:
    End(PosDir, QString, QString);
    End();
    bool operator<(const End& right) const;
    QString bandId();
    QString theOtherId();
    bool isClose(EndPtr&);
    void setCouple(EndPtr&);
    void setCouple(End&);
    bool isCoupled();
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
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
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
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
    GLIndexPairVec getGLIndexPairVec(EndPtr, float);
    void setShowRange(GLIndexPair);
    void resetShowRange();
    PosDir indexPd(int);
    PosDirVec indexsPds(vector<int>);
    Dir outNorm(int);
    QString beginFlag();
protected:
    QString m_begin;
    BandEndPtr m_end;
    PosDirVec m_pds;
    DirVec m_norms;
    QStringVec m_strs;
    Mesh mesh;
    vector<CurvePtr> curves;
    unsigned int m_numPerPd;
};

using BandPtr = std::shared_ptr<Band>;

#endif // BAND_H

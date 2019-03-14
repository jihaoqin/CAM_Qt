#include "TCurve.h"
#include "Point.h"
#include "Tee.h"
#include "RingAssist.h"
#include "CPPara.h"
#include "RingCurveAssist.h"

TCurve::TCurve(PointPtr p, float a, TeePtr t):designP(p), uAng(a), tee(t)
{
    QString meshType(p->meshId());
    if(meshType.contains("ring")){
        Ring* r = tee->getRing(meshType);
        assert(r);
        RingAssist assist(*r);
        vector<float> uv = assist.paraWithLocalPoint(designP->getPos());
        CPPara para{uv.at(0), uv.at(1), uAng};
        glm::vec3 dir = assist.worldDir(para);
    }
    else if(meshType.contains("plane")){

    }
    else if(meshType.contains("cylinder")){

    }
}

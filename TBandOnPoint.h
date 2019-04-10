#ifndef TBANDONPOINT_H
#define TBANDONPOINT_H
#include "Band.h"
#include "Point.h"
#include "TCurve.h"
#include "Mesh.h"

class TeeCurve;
using TeeCurvePtr = std::shared_ptr<TeeCurve>;

class TBandOnPoint:public Band
{
public:
    TBandOnPoint(PointPtr p, float w, TCurvePtr tc, QString name, TeePtr t);
    virtual void bindGL(QOpenGLContext* c) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
    void setColor(Color) override;
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;
protected:
    virtual void updateSelf() override;
private:
    TeePtr tee;
    PointPtr point;
    float width;
    TCurvePtr tcurve;
    Color color;
    QOpenGLFunctions_4_3_Core *core;
};

using TBandOnPointPtr = std::shared_ptr<TBandOnPoint>;
#endif // TBANDONPOINT_H

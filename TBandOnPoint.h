#ifndef TBANDONPOINT_H
#define TBANDONPOINT_H
#include "Band.h"
#include "Point.h"
#include "TCurve.h"
#include "Mesh.h"

class TBandOnPoint:public Band
{
public:
    TBandOnPoint(PointPtr p, float w, TCurvePtr tc, QString name, TeePtr t);
    virtual void bindGL(QOpenGLContext* c) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();

protected:
    virtual void updateSelf() override;
private:
    TeePtr tee;
    PointPtr point;
    float width;
    TCurvePtr tcurve;
    Mesh mesh;
};

#endif // TBANDONPOINT_H

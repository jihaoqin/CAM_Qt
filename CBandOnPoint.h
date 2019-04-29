#ifndef CBANDONPOINT_H
#define CBANDONPOINT_H
#include "Band.h"
#include "Mesh.h"
#include "PosDir.h"
#include "HalfPoint.h"

class CBandOnPoint:public Band
{
public:
    CBandOnPoint(HalfPoint, float angle, TeePtr, QString, float w);
    virtual void bindGL(QOpenGLContext*);
    virtual void draw(std::shared_ptr<GLProgram>);
    void setPara(HalfPoint, float angle);
    QString frontMeshId();
protected:
    virtual void updateSelf();
private:
    void initial(PosDirVec pds, QStringVec names);
    TeePtr tee;
    Mesh m;
    HalfPoint m_point;
    float windingAngle;
    float width;
    Color color;
    QOpenGLFunctions_4_3_Core *core;
};

using CBandPtr = std::shared_ptr<CBandOnPoint>;
#endif // CBANDONPOINT_H

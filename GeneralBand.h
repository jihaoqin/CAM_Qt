#ifndef GENERALBAND_H
#define GENERALBAND_H
#include "Band.h"
#include "PosDir.h"
#include "Mesh.h"
#include <vector>

class GeneralBand:public Band
{
public:
    GeneralBand(PosDirVec pds, QStringVec names, QString name, TeePtr t);
    virtual void bindGL(QOpenGLContext* c) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
    void setColor(Color) override;
private:
    void initial(PosDirVec pds, QStringVec names);
    float width;
    TeePtr tee;
    Color color;
    QOpenGLFunctions_4_3_Core *core;
};

using GeneralBandPtr = std::shared_ptr<GeneralBand>;
#endif //

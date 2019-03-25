#ifndef BAND_H
#define BAND_H

#include "DataObject.h"
#include "PosDir.h"
#include "Curve.h"

class Tee;
using Pos = glm::vec3;
using QStringVec = std::vector<QString>;
using TeePtr = std::shared_ptr<Tee>;

class Band:public DataObject
{
public:
    Band();
    virtual void setColor(Color);
private:
};

using BandPtr = std::shared_ptr<Band>;

#endif // BAND_H

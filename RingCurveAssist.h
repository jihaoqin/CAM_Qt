#ifndef RINGCURVEASSIST_H
#define RINGCURVEASSIST_H
#include <glm/glm.hpp>
#include "Ring.h"
#include "RingAssist.h"
#include "PosDir.h"
#include "CPPara.h"
#include <utility>
#include "Edge.h"
using std::pair;
using std::tuple;
class RingCurveAssist
{
public:
    RingCurveAssist(Ring& ring);
    tuple<std::vector<PosDir>, vector<EdgePtr>, float> genCurve(glm::vec3 pos, glm::vec3 dir, float coe, float length = 1e4);
    tuple<std::vector<PosDir>, vector<EdgePtr>, float> genCurve(glm::vec3 pos, float uAng, float coe, float length = 1e6);
private:
    float length3D(CPPara, CPPara);
    vector<float> ringDiff(float s, vector<float> y0, float coe);
    tuple<vector<CPPara>, vector<EdgePtr>, float> genCurve(CPPara, float, float length);
    tuple<vector<CPPara>, EdgePtr, float> rungeKutta(vector<float> xspan, CPPara y0, float lam);
    void initial(Ring&);
    CPPara evalNextRunge(float x, CPPara p0, float h, float coe);

    RingAssist assist;
    double R;
    double r;
    double angle;//弧度制
    QString id;
    vector<EdgePtr> edges;
    glm::vec3 anchor;
    glm::vec3 zdir;
    glm::vec3 xdir;
};

#endif // RINGCURVEASSIST_H

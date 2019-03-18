#ifndef RINGCURVEASSIST_H
#define RINGCURVEASSIST_H
#include <glm/glm.hpp>
#include "Ring.h"
#include "RingAssist.h"
#include "PosDir.h"
#include "CPPara.h"
#include <utility>
using std::pair;

class RingCurveAssist
{
public:
    RingCurveAssist(Ring& ring);
    pair<std::vector<PosDir>, vector<EdgePtr>> genCurve(glm::vec3 pos, glm::vec3 dir, float coe);
    pair<std::vector<PosDir>, vector<EdgePtr>> genCurve(glm::vec3 pos, float uAng, float coe);
private:
    vector<float> ringDiff(float s, vector<float> y0);
    typedef vector<float> Ys;
    pair<vector<CPPara>, vector<EdgePtr>> genCurve(CPPara);
    pair<vector<CPPara>, EdgePtr> rungeKutta(vector<float> xspan, Ys y0);
    void initial(Ring&);
    vector<float> evalNextRunge(float x, vector<float> y0, float h);

    double lambda;
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

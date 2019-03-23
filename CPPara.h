#ifndef CPPARA_H
#define CPPARA_H
#include <vector>

class CPPara
{// curve point parameter, include u, v and angle about tangent u and tangent dir;

public:
    float u;
    float v;
    float uAng;
};
using CPParaVec = std::vector<CPPara>;
#endif // CPPARA_H

#ifndef MOVEDATA_H
#define MOVEDATA_H
#include <vector>


class Axis4Data{
public:
    float theta;//主轴回转
    float x;//横向平移
    float z;//纵向平移
    float flip;//翻滚坐标
};
class Axis5Data{
public:
    float theta;//主轴回转
    float x;//横向平移
    float z;//纵向平移
    float flip;//翻滚坐标
    float yaw;//偏摆坐标
};

using Axis4DataVec = std::vector<Axis4Data>;
using Axis5DataVec = std::vector<Axis5Data>;

#endif // MOVEDATA_H

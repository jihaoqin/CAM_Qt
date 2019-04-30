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
class AxisMoveData{
public:
    AxisMoveData(int);
    int axisSum();
    float x();
    float x(float);
    float z();
    float z(float);
    float flip();
    float flip(float);
    float theta();
    float theta(float);
    float yaw();
    float yaw(float);
private:
    int m_axisSum;
    float m_theta;//主轴回转
    float m_x;//横向平移
    float m_z;//纵向平移
    float m_flip;//翻滚坐标
    float m_yaw;//偏摆坐标
};

using Axis4DataVec = std::vector<Axis4Data>;
using Axis5DataVec = std::vector<Axis5Data>;
using AxisMoveDataVec = std::vector<AxisMoveData>;

#endif // MOVEDATA_H

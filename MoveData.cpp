#include "MoveData.h"
#include <assert.h>

AxisMoveData::AxisMoveData(int num){
    m_axisSum = num;
}

float& AxisMoveData::x(){
    return m_x;
}

float& AxisMoveData::x(float v){
    m_x = v;
    return m_x;
}

float& AxisMoveData::z(){
    return m_z;
}

float& AxisMoveData::z(float v){
    m_z = v;
    return m_z;
}

float& AxisMoveData::flip(){
    return m_flip;
}

float& AxisMoveData::flip(float v){
    m_flip = v;
    return m_flip;
}

float& AxisMoveData::theta(){
    return m_theta;
}

float& AxisMoveData::theta(float v){
    m_theta = v;
    return m_theta;
}

float& AxisMoveData::yaw(){
    assert(m_axisSum == 5);
    return m_yaw;
}

float& AxisMoveData::yaw(float v){
    assert(m_axisSum == 5);
    m_yaw = v;
    return m_yaw;
}

int AxisMoveData::axisSum(){
    return m_axisSum;
}

#ifndef AXISINI_H
#define AXISINI_H
#include "NameDeclare.h"

class AxisIni{
public:
    enum{
        xLeft = 	0x0000001,
        zDown = 	0x0000010,
        flipDown = 	0x0000100,
        spindleLeft=0x0001000,
        yawLeft =	0x0010000,
    };
    AxisIni(int);
    QString offStr(int);
    QString showStr(int);
    QString name(int);
    QString machineName();
    void setAxisSum(int);
    int axisSum();
    int config;
private:
    QStringVec names;
    QString machine;
    std::vector<float> axis_Offs;
};


#endif // AXISINI_H

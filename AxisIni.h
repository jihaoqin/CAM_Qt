#ifndef AXISINI_H
#define AXISINI_H
#include "NameDeclare.h"

class AxisIni{
public:
    AxisIni(int);
    QString offStr(int);
    QString showStr(int);
    QString name(int);
    QString machineName();
    int axisSum();
    QStringVec names;
    QString machine;
    std::vector<float> axis_Offs;
};


#endif // AXISINI_H

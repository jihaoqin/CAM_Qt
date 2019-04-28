#ifndef AXISINI_H
#define AXISINI_H
#include "NameDeclare.h"
#include "rapidjson/prettywriter.h"

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
    void setName(int, QString);
    void setOff(int, float);
    QString machineName();
    void machineName(QString);
    QString tabMachineName();
    QString tabAxis(int);
    void setAxisSum(int);
    int axisSum() const;
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
public:
    int config;
private:
    QStringVec names;
    QString machine;
    std::vector<float> axis_Offs;
};


#endif // AXISINI_H

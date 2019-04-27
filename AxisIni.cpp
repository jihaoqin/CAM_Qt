#include "AxisIni.h"

AxisIni::AxisIni(int num)
    :names(num, "1"), machine("untitled"), axis_Offs(num, 0), config(0)
{
}

QString AxisIni::offStr(int ind){
    if(ind >= axis_Offs.size()){
        return QString("");
    }
    else{
        return QString::number(axis_Offs.at(ind));
    }
}

QString AxisIni::showStr(int ind){
    if(ind >= axis_Offs.size()){
        return QString("Axis null");
    }
    else{
        return QString("Axis") + name(ind) + ":" + offStr(ind);
    }
}

QString AxisIni::name(int ind){
    if(ind >= axis_Offs.size()){
        return QString("");
    }
    else{
        return names.at(ind);
    }
}

QString AxisIni::machineName(){
    return machine;
}


int AxisIni::axisSum(){
    return axis_Offs.size();
}


QString AxisIni::setAxisSum(int num){

}

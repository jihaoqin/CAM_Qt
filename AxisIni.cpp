#include "AxisIni.h"

AxisIni::AxisIni(int num)
    :names(num, "untitled"), machine("untitled"), axis_Offs(num, 0), config(0)
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


void AxisIni::setAxisSum(int num){
    assert(axis_Offs.size() == 4 || axis_Offs.size() == 5);
    if(num == 4){
        if(axis_Offs.size() == 4){
            //do nothing
        }
        else if(axis_Offs.size() == 5){
            axis_Offs.pop_back();
            names.pop_back();
        }
        else{
            assert(0);
        }
    }
    else if(num == 5){
        if(axis_Offs.size() == 4){
            names.push_back("untitled");
            axis_Offs.push_back(0);
        }
        else if(axis_Offs.size() == 5){
            //do nothing
        }
        else{
            assert(0);
        }
    }
    else{
        assert(0);
    }
}

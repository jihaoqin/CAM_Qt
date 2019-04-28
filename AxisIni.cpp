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

void AxisIni::setName(int ind, QString s){
    names.at(ind) = s;
}

void AxisIni::setOff(int ind, float v){
    axis_Offs.at(ind) = v;
}

QString AxisIni::machineName(){
    return machine;
}

void AxisIni::machineName(QString s){
    machine = s;
}

int AxisIni::axisSum() const{
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

void AxisIni::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
    writer.StartObject();
    writer.String("type");
    writer.String("AxisIni");
    writer.String("machine");
    writer.String(machine.toLatin1().data());
    writer.String("axisSum");
    writer.Int(axisSum());
    writer.String("axis_Offs");
    writer.StartArray();
    for(auto off:axis_Offs){
        writer.Double(off);
    }
    writer.EndArray();
    writer.String("names");
    writer.StartArray();
    for(auto name:names){
        writer.String(name.toLatin1().data());
    }
    writer.EndArray();
    writer.String("config");
    writer.Int(config);
    writer.EndObject();
}


QString AxisIni::tabMachineName(){
    return QString("Machine: ") + machine;
}

QString AxisIni::tabAxis(int ind){
    return QString("Axis ") + names.at(ind) + ": " + QString::number(axis_Offs.at(ind));
}


float AxisIni::off(int ind){
    return axis_Offs.at(ind);
}

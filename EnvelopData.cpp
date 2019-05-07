#include "EnvelopData.h"

EnvelopData::EnvelopData()
    :incPipeR(0), incMLength(0), incBLength(0)
{

}


void EnvelopData::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
    writer.StartObject();
    writer.String("type");
    writer.String("EnvelopData");
    writer.String("incPipeR");
    writer.Double(incPipeR);
    writer.String("incBLength");
    writer.Double(incBLength);
    writer.String("incMLength");
    writer.Double(incMLength);
    writer.EndObject();
}

EnvelopData::EnvelopData(float incR_, float incBL_, float incML_){
    incPipeR = incR_;
    incBLength = incBL_;
    incMLength = incML_;
}

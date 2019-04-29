#include "EnvelopData.h"

EnvelopData::EnvelopData()
    :incPipeR(0), incLength(0)
{

}


void EnvelopData::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
    writer.StartObject();
    writer.String("type");
    writer.String("EnvelopData");
    writer.String("incPipeR");
    writer.Double(incPipeR);
    writer.String("incLength");
    writer.Double(incLength);
    writer.EndObject();
}

EnvelopData::EnvelopData(float incR_, float incL_){
    incPipeR = incR_;
    incLength = incL_;
}

#ifndef ENVELOPDATA_H
#define ENVELOPDATA_H
#include "rapidjson/prettywriter.h"

class EnvelopData
{
public:
    EnvelopData();
    EnvelopData(float incR_, float incB_, float incM_);
    float incPipeR;
    float incBLength;
    float incMLength;
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
};

#endif // ENVELOPDATA_H

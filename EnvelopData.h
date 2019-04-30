#ifndef ENVELOPDATA_H
#define ENVELOPDATA_H
#include "rapidjson/prettywriter.h"

class EnvelopData
{
public:
    EnvelopData();
    EnvelopData(float , float );
    float incPipeR;
    float incLength;
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
};

#endif // ENVELOPDATA_H

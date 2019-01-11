#include "Data.h"
using std::shared_ptr;
Data::Data()
{

}

void Data::addModel(shared_ptr<Model> m){
    modelVec.push_back(m);
}
void Data::addLine(std::shared_ptr<Line> l){
   lineVec.push_back(l);
}

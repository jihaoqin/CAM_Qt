#include "DataState.h"
#include "Data.h"

DataState::DataState():changed(false), empty(true){

}

DataState::DataState(Data* d):changed(false)
{
    if(d->hasTee()){
        empty = true;
    }
    else{
        empty = false;
    }
}

bool DataState::getEmpty(){
    return empty;
}

void DataState::setEmpty(bool flag){
    empty = flag;
}

bool DataState::getChanged(){
    return changed;
}

void DataState::setChanged(bool flag){
    changed = flag;
}

#ifndef DATASTATE_H
#define DATASTATE_H

class Data;
class DataState
{
public:
    DataState();
    DataState(Data *);
    void setChanged(bool);
    bool getChanged();
    void setEmpty(bool);
    bool getEmpty();
private:
    bool changed;
    bool empty;
};

#endif // DATASTATE_H

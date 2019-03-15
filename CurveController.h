#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H
#include <QString>

class Controller;
class CurveController
{
public:
    CurveController(Controller*);
    void updateCurve(QString, float);
private:
    Controller* ctrl;
};

#endif // CURVECONTROLLER_H

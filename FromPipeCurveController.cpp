#include "FromPipeCurveController.h"
#include "Node.h"
#include "CBandOnPoint.h"

FromPipeCurveController::FromPipeCurveController(FromPipeCurveTab* t)
    :tab(t)
{

}

void FromPipeCurveController::updateBandUsing(QString pointId, QString bandId){
    BandPtr bandPtr = data->getNodeRoot()->findBandPtr(bandId);
    CBandPtr cBand = std::dynamic_pointer_cast<CBandOnPoint>(bandPtr);
    cBand->setPara();
}

#include "transition.h"

Transition::Transition(double ts, int posX, int posY, int polarityType)
{
    timeStamp = ts;
    x = posX;
    y = posY;
    type = polarityType;
}

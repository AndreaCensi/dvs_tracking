#include "interval.h"

Interval::Interval(unsigned int ts, int posX, int posY, unsigned int dT)
{
    timeStamp = ts;
    x = posX;
    y = posY;
    deltaT = dT;
}

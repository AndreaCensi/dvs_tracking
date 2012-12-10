#include "interval.h"

Interval::Interval(double ts, int posX, int posY, double dT)
{
    timeStamp = ts;
    x = posX;
    y = posY;
    deltaT = dT;
}

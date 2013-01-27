#include "interval.h"

//! Constructor
/*!
    \param ts The timestamp of the interval.
    \param posX x coordinate of Pixel.
    \param posY y coordinate of Pixel.
    \param dT The time interval in seconds.
*/
Interval::Interval(double ts, int posX, int posY, double dT)
{
    timeStamp = ts;
    x = posX;
    y = posY;
    deltaT = dT;
}

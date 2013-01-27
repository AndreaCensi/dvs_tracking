#ifndef INTERVAL_H
#define INTERVAL_H

//! A container for a per-pixel interspike interval
/*!
    \author Jonas Strubel
*/
class Interval
{
public:
    Interval(double ts = 0, int posX = -1, int posY = -1, double dT = 0);
    double timeStamp;
    int x;
    int y;
    double deltaT;
};

#endif // INTERVAL_H

#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
    Interval(unsigned int ts = 0, int posX = -1, int posY = -1, unsigned int dT = 0);
    unsigned int timeStamp;
    int x;
    int y;
    unsigned int deltaT;
    float weight;
};

#endif // INTERVAL_H

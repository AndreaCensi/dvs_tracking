#ifndef MAXIMA_H
#define MAXIMA_H

#include "localmaximum.h"

//! A container for local maxima
/*!
    \author Jonas Strubel
*/
class Maxima
{
public:
    Maxima(int numberOfMaxima, float minimumDistance);
    ~Maxima();
    void update(int x, int y, float weight);
    LocalMaximum* get(int i);
    void reset();
    int size();

private:
    float squaredDistance(LocalMaximum *m, int x, int y);

    LocalMaximum *maxima;
    int numMaxima;
    float minSquaredDistance;
};

#endif // MAXIMA_H

#ifndef MAXIMA_H
#define MAXIMA_H

#include "localmaximum.h"

class Maxima
{
public:
    Maxima(int numberOfMaxima, float minimumDistance);
    ~Maxima();
    void update(int x, int y, int weight);
    LocalMaximum* get(int i);
    void reset();
    int size();

private:
    float squaredDistance(LocalMaximum *m, int x, int y);

    LocalMaximum *maxima;
    int numMaxima;
    float minDistance;
};

#endif // MAXIMA_H

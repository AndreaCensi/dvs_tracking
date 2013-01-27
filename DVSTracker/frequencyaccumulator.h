#ifndef FREQUENCYACCUMULATOR_H
#define FREQUENCYACCUMULATOR_H

#include "map.h"
#include "interval.h"
#include "filter.h"
#include "vector"
#include "maxima.h"

//! A class for gathering and evaluating per-pixel interspike intervals
/*!
    Interspike intervals are gathered and weighted in terms of there distance to a certain frequency one wants to measure. Weights are saved on a pixelmap
    during a certain period of time before being evaluated.
    \author Jonas Strubel
*/
class FrequencyAccumulator
{
public:
    FrequencyAccumulator(int frequency = 0, float measurePeriod = 0.01f, float sigma = 1.0, int filterSize = 5, float filterSigma = 1.0,
                         float minDist = 0, int numMaxima = 0, int w = 128, int h = 128);
    ~FrequencyAccumulator();
    void update(Interval interval);
    bool hasExpired();
    Maxima* getMaxima();
    Map<float> *weightMap;
    void reset();

    Maxima *maxima;

    static const float PI;

//private:
    float getWeight(double interval, int frequency, float sd);

    int targetFrequency;
    float measuringPeriod;
    float sd;
    float minDistance;
    int nMaxima;
    double lastReset;
    double lastUpdate;

    Filter *filter;
};

#endif // FREQUENCYACCUMULATOR_H

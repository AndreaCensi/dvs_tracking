#ifndef FREQUENCYACCUMULATOR_H
#define FREQUENCYACCUMULATOR_H

#include "map.h"
#include "interval.h"
#include "filter.h"
#include "vector"
#include "maxima.h"

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

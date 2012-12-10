#ifndef FREQUENCYACCUMULATOR_H
#define FREQUENCYACCUMULATOR_H

#include "map.h"
#include "interval.h"
#include "filter.h"
#include "vector"

class FrequencyAccumulator
{
public:
    FrequencyAccumulator(int frequency, float sigma, int filterSize, float filterVariance, float minDist, int w = 128, int h = 128);
    ~FrequencyAccumulator();
    void update(Interval interval);
    bool hasExpired();
    std::vector evaluate();
    void reset();

    static const float PI;

private:
    float getWeight(Interval interval, int frequency, double variance);
    std::vector findMaxima();

    Map<float> *weightMap;
    int targetFrequency;
    float variance;
    float minDistance;
    double lastReset;
    double lastUpdate;

    Filter *filter;
};

#endif // FREQUENCYACCUMULATOR_H

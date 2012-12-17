#ifndef FREQUENCYACCUMULATOR_H
#define FREQUENCYACCUMULATOR_H

#include "map.h"
#include "interval.h"
#include "filter.h"
#include "vector"
#include "localmaximum.h"

class FrequencyAccumulator
{
public:
    FrequencyAccumulator(int frequency = 0, float sigma = 1.0, int filterSize = 5, float filterSigma = 1.0,
                         float minDist = 0, int numMaxima = 0, int w = 128, int h = 128);
    ~FrequencyAccumulator();
    void update(Interval interval);
    bool hasExpired();
    bool isEvaluated();
    void evaluate();

    Map<int> *weightMap;
    Map<int> **buffers;
    Map<int> *filteredMap;
    Map<int> *lastBuffer;

    void reset();

    static const float PI;

private:
    int getWeight(double interval, int frequency, float sd);
    std::vector<LocalMaximum> findMaxima();
    void switchBuffer();

    int bufferIndex;

    int targetFrequency;
    float sd;
    float minDistance;
    int nMaxima;
    double lastReset;
    double lastUpdate;

    Filter *filter;
};

#endif // FREQUENCYACCUMULATOR_H

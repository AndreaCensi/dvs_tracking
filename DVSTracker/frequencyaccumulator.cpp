#include "frequencyaccumulator.h"

const float FrequencyAccumulator::PI = 3.14159265f;

FrequencyAccumulator::FrequencyAccumulator(int frequency, float sigma, int filterSize, float filterVariance, float minDist, int w, int h)
{
    // Weights
    weightMap = new Map<float>[w*h];
    targetFrequency = frequency;
    variance = sigma;
    lastReset = 0;
    lastUpdate = 0;

    //Maxima search
    minDistance = minDist;

    // Smoohing filter
    filter = new Filter(filterSize,filterVariance);
}

FrequencyAccumulator::~FrequencyAccumulator(){
    delete weightMap;
    delete filter;
}

void FrequencyAccumulator::update(Interval interval){
    lastUpdate = interval.timeStamp;

    int x = interval.x;
    int y = interval.y;
    float prevWeight = weightMap->get(x,y);
    float weight = getWeight(interval.deltaT,targetFrequencies.at(i),variance) + prevWeight;
    weightMap->insert(x,y,weight);
}

float FrequencyAccumulator::getWeight(Interval interval, int frequency, double variance){
    double targetInterval = 1.0/frequency;
    double tDiff = targetInterval - interval.deltaT;
    float weight = float(1.0/(variance*sqrt(2*PI)*exp(-1/2*pow(tDiff/variance),2)));
    return weight;
}

bool FrequencyAccumulator::hasExpired(){
    if( (lastUpdate - lastReset) > (1.0/targetFrequency) )
        return true;
    else
        return false;
}

std::vector FrequencyAccumulator::evaluate(){
    weightMap = filter->smoothen(weightMap);
    std::vector maxima = findMaxima();
    return maxima;
}

void FrequencyAccumulator::reset(){
    weightMap->reset();
    lastReset = lastUpdate;
}

std::vector FrequencyAccumulator::findMaxima(){
    // search for maxima in the map, use minDistance!
}

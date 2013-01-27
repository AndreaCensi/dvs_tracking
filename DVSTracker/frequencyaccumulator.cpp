#include "frequencyaccumulator.h"

const float FrequencyAccumulator::PI = 3.14159265f;

#define WEIGHT_MULTIPLIER 1

//! Constructor
/*!
    \param frequency Target frequency to be measured.
    \param measurePeriod Interval in which to measure interspike intervals.
    \param sigma The standard deviation for the weighting
    \param filterSize Size of smoothing filter - NOT IN USE.
    \param filterSigma Standard deviation of smoothing filter - NOT IN USE.
    \param minDist Minimum distance between local maxima in the weight map.
    \param numMaxima Number of local maxima to gather from the weight map.
    \param w Width of the weight map ( The same as the camera image width).
    \param h Height of the weight map ( The same as the camera image height).
*/
FrequencyAccumulator::FrequencyAccumulator(int frequency, float measurePeriod,
                                           float sigma, int filterSize, float filterSigma,
                                           float minDist, int numMaxima, int w, int h)
{
    // Weights
    weightMap = new Map<float>(w,h);
    for(int i = 0; i < weightMap->size();i++)
        weightMap->set(i,0);

    maxima = new Maxima(numMaxima,minDist);

    targetFrequency = frequency;
    measuringPeriod = measurePeriod;
    sd = sigma;
    lastReset = 0;
    lastUpdate = 0;

    //Maxima search
    minDistance = minDist;
    nMaxima = numMaxima;

    // Smoohing filter
    filter = new Filter(filterSize,filterSigma, w, h);
}

FrequencyAccumulator::~FrequencyAccumulator(){    
    delete weightMap;
    delete maxima;
    delete filter;
}

void FrequencyAccumulator::update(Interval interval){
    lastUpdate = interval.timeStamp;
    int x = interval.x;
    int y = interval.y;
    float prevWeight = weightMap->get(x,y);
    float weight = getWeight(interval.deltaT,targetFrequency,sd) + prevWeight;

//    printf("dt: %f, w: %f\n",interval.deltaT,weight);

    weightMap->insert(x,y,weight);
    //udpate maxima
    maxima->update(x,y,weight);
}

float FrequencyAccumulator::getWeight(double interval, int frequency, float sd){
    float measuredFrequency = 1.0/interval;
    float diff = measuredFrequency - float(frequency);
    float weight = 1.0f/(sd*sqrt(2*PI)) * exp(-pow(diff/sd,2.0f))/2.0f;
    return weight*WEIGHT_MULTIPLIER;
}

bool FrequencyAccumulator::hasExpired(){
    if( (lastUpdate - lastReset) > measuringPeriod)
        return true;
    else
        return false;
}

void FrequencyAccumulator::reset(){
    // reset values
    for(int i = 0; i < weightMap->size();i++)
        weightMap->set(i,0);
    maxima->reset();
    lastReset = lastUpdate;
}

Maxima* FrequencyAccumulator::getMaxima(){
    return maxima;
}

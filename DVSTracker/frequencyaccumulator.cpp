#include "frequencyaccumulator.h"

const float FrequencyAccumulator::PI = 3.14159265f;

#define N_GUESSES 16
#define WEIGHT_MULTIPLIER 100000

FrequencyAccumulator::FrequencyAccumulator(int frequency, float periodMultiplier,
                                           float sigma, int filterSize, float filterSigma,
                                           float minDist, int numMaxima, int w, int h)
{
    // Weights
    weightMap = new Map<int>(w,h);
    for(int i = 0; i < weightMap->size();i++)
        weightMap->set(i,0);

    maxima = new Maxima(numMaxima,minDist);

    targetFrequency = frequency;
    multiplier = periodMultiplier;
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
    int prevWeight = weightMap->get(x,y);
    int weight = getWeight(interval.deltaT,targetFrequency,sd) + prevWeight;

//    printf("dt: %f, w: %f\n",interval.deltaT,weight);

    weightMap->insert(x,y,weight);
    //udpate maxima
    maxima->update(x,y,weight);
}

int FrequencyAccumulator::getWeight(double interval, int frequency, float sd){
    float measuredFrequency = 1.0/interval;
    float diff = measuredFrequency - float(frequency);
    float weight = 1.0f/(sd*sqrt(2*PI)) * exp(-pow(diff/sd,2.0f))/2.0f;
    return int(WEIGHT_MULTIPLIER*weight);
}

bool FrequencyAccumulator::hasExpired(){
    if( (lastUpdate - lastReset) > (multiplier/targetFrequency) )
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

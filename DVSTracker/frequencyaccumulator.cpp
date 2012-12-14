#include "frequencyaccumulator.h"

const float FrequencyAccumulator::PI = 3.14159265f;

#define N_GUESSES 16

FrequencyAccumulator::FrequencyAccumulator(
        int frequency, float sigma, int filterSize,
        float filterSigma, float minDist, int numMaxima, int w, int h)
{
    // Weights
    weightMap = new Map<float>(w,h);
    for(int i = 0; i < weightMap->size();i++)
        weightMap->set(i,0);

    targetFrequency = frequency;
    sd = sigma;
    lastReset = 0;
    lastUpdate = 0;

    //Maxima search
    minDistance = minDist;
    nMaxima = numMaxima;

    // Smoohing filter
    filter = new Filter(filterSize,filterSigma);
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
    float weight = getWeight(interval.deltaT,targetFrequency,sd) + prevWeight;
    weightMap->insert(x,y,weight);
}

float FrequencyAccumulator::getWeight(double interval, int frequency, float sd){
    double targetInterval = 1.0/frequency;
    double tDiff = targetInterval - interval;
    float weight = float(1.0/(sd*sqrt(2*PI)) * exp(-pow(tDiff/sd,2.0))/2.0);
    return weight;
}

bool FrequencyAccumulator::hasExpired(){
    if( (lastUpdate - lastReset) > (1.0/targetFrequency) )
        return true;
    else
        return false;
}

std::vector<LocalMaximum> FrequencyAccumulator::evaluate(){
    weightMap = filter->smoothen(weightMap);
    std::vector<LocalMaximum> maxima = findMaxima();
    return maxima;
}

void FrequencyAccumulator::reset(){
    // reset values
    for(int i = 0; i < weightMap->size();i++)
        weightMap->set(i,0);
    lastReset = lastUpdate;
}

std::vector<LocalMaximum> FrequencyAccumulator::findMaxima(){
    // search for maxima in the map, use minDistance!
    std::vector<LocalMaximum> maxima;
    for(int h = 0; h < weightMap->height; h++){
        for(int w = 0; w < weightMap->width; w++){
            // don't process if no weight has been added
            if(weightMap->get(w,h) == 0.0f)
                continue;
            // find maxima
        }
    }
    return maxima;
}

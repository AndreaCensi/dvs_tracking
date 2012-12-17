#include "frequencyaccumulator.h"

const float FrequencyAccumulator::PI = 3.14159265f;

#define N_GUESSES 16
#define NUM_BUFFERS 3

FrequencyAccumulator::FrequencyAccumulator(
        int frequency, float sigma, int filterSize,
        float filterSigma, float minDist, int numMaxima, int w, int h)
{
    // Weights
    buffers = new Map<int>*[NUM_BUFFERS];
    for(int i = 0; i < NUM_BUFFERS; i++){
        buffers[i] = new Map<int>(w,h);
        weightMap = buffers[i];
        for(int i = 0; i < weightMap->size();i++)
            weightMap->set(i,0);
    }

    bufferIndex = 0;
    weightMap = buffers[bufferIndex];
    filteredMap = 0;

    //parameters
    targetFrequency = frequency;
    sd = sigma;

    //interval measurement
    lastReset = 0;
    lastUpdate = 0;

    //Maxima search
    minDistance = minDist;
    nMaxima = numMaxima;

    // Smoohing filter
    filter = new Filter(filterSize,filterSigma, w, h);
    filter->start();
}

FrequencyAccumulator::~FrequencyAccumulator(){
    filter->stop();
    filter->wait();
    for(int i = 0; i < NUM_BUFFERS; i++)
        delete buffers[i];
    delete [] buffers;
    delete filter;
}

void FrequencyAccumulator::update(Interval interval){
    lastUpdate = interval.timeStamp;
    int x = interval.x;
    int y = interval.y;
    int prevWeight = weightMap->get(x,y);
    int weight = getWeight(interval.deltaT,targetFrequency,sd) + prevWeight;
    weightMap->insert(x,y,weight);
}

int FrequencyAccumulator::getWeight(double interval, int frequency, float sd){
    double targetInterval = 1.0/frequency;
    double tDiff = targetInterval - interval;
    int weight = int(1.0/(sd*sqrt(2*PI)) * exp(-pow(tDiff/sd,2.0))/2.0);
    return weight;
}

bool FrequencyAccumulator::hasExpired(){
    if( (lastUpdate - lastReset) > (1.0/targetFrequency) )
        return true;
    else
        return false;
}

bool FrequencyAccumulator::isEvaluated(){
    return filter->isDone();
}

void FrequencyAccumulator::evaluate(){
    filter->setMap(weightMap);  // set map to filter
    filter->resume();   // start filtering
    filteredMap = filter->getFilteredMap(); //get filtered map
    switchBuffer(); //

//    weightMap = filter->smoothen(weightMap);
//    std::vector<LocalMaximum> maxima = findMaxima();
//    return maxima;
}

void FrequencyAccumulator::reset(){
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

void FrequencyAccumulator::switchBuffer(){
    if(bufferIndex == NUM_BUFFERS-1)
        bufferIndex = 0;
    else
        bufferIndex++;
    weightMap = buffers[bufferIndex];
}

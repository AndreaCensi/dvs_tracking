#include "tracker.h"
#include "stdio.h"
#include "maxima.h"
#include "localmaximum.h"
#include <math.h>

// Resolution of DVS
#define DVS_RES 128

// Parameteres
#define SIGMA_W 0.000008f // sigma for perdiod weighting

// Guassian smoothing filter
#define FILTER_SIZE 3   // kernel size
#define SIGMA_FILTER 0.75f

// Maximum detection
#define MIN_DIST 4.0f   // min distance between maxima
#define NUM_MAXIMA 3

// Particle filter parameters
#define PF_NUM_PARTICLES 8
#define PF_DEFAULT_SIGMA 5.0f
#define PF_MAX_SIGMA 16.0f
#define PF_V_MAX 25.0f

// Combination analysis
#define CA_MIN_DIST 8.0f
#define CA_NUM_HYPOTHESIS 8

Tracker::Tracker(PacketBuffer *buffer, std::vector<int> frequencies, QObject *parent) : QThread(parent){
    //Members
    packetBuffer = buffer;
    targetFrequencies = frequencies;

    widget = 0;

    exit = false;

    // init maps
    latestEvents = new Map<Event>(DVS_RES,DVS_RES);
    npTransitions = new Map<Transition>(DVS_RES,DVS_RES);
    pnTransitions = new Map<Transition>(DVS_RES,DVS_RES);

    weightBuffers = new FrequencyAccumulator*[targetFrequencies.size()];
    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        weightBuffers[i] = new FrequencyAccumulator(
                    targetFrequencies[i],SIGMA_W,FILTER_SIZE,SIGMA_FILTER,MIN_DIST,NUM_MAXIMA,DVS_RES,DVS_RES);
    }

    particleFilters = new ParticleFilter*[targetFrequencies.size()];
    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        particleFilters[i] = new ParticleFilter(PF_NUM_PARTICLES,PF_DEFAULT_SIGMA,PF_MAX_SIGMA,PF_V_MAX);
    }

    logger = new HypothesisLogger("C:/Users/giselher/Documents/uzh/hypo_log.txt");
    lastEventTs = 0;
    eventCount = 0;

    combinationAnalyzer = new CombinationAnalyzer(particleFilters,targetFrequencies.size(),CA_MIN_DIST,CA_NUM_HYPOTHESIS);
}

Tracker::~Tracker(){
    delete latestEvents;
    delete npTransitions;
    delete pnTransitions;

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        delete weightBuffers[i];
    }
    delete [] weightBuffers;

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        delete particleFilters[i];
    }
    delete [] particleFilters;

    delete logger;
}

void Tracker::processEvent(Event e){
    // Record, if there is a transition
    Transition t = getTransition(e);
    if(t.timeStamp == 0){
        return;
    }

    // Get interval to last transition
    Interval dt = getInterval(t);
    if(dt.timeStamp == 0 || dt.deltaT < 0){
        return;
    }

    //stop logger
    if(lastEventTs > e.timeStamp && !logger->done()){
        printf("#Events(tra): %d\n",eventCount);
        eventCount = 0;
        //logger->stop();
    }
    else
        eventCount++;

    //Calculate importance of interval for each frequency
    for(unsigned int i = 0; i < targetFrequencies.size(); i++){
        FrequencyAccumulator *buf = weightBuffers[i];
        buf->update(dt);
        if(buf->hasExpired()){
            Maxima *maxima = buf->getMaxima();
            //process maxima here
            //for(int j = 0; j < targetFrequencies.size();j++){
            ParticleFilter *pf = particleFilters[i];
            pf->update(maxima,e.timeStamp);

//            widget->updateMaxWeightParticle(i,pf->getMinUncertaintyParticle());
            //}

            //            if(!logger->done()){
            //                for(int j = 0; j < maxima->size();j++)
            //                    logger->log(e.timeStamp,targetFrequencies[i],maxima->size(),
            //                                j,maxima->get(j)->x,maxima->get(j)->y,maxima->get(j)->weight);
            //            }

            updateWeightWidget(i,buf,maxima);
            buf->reset();
        }
    }
    lastEventTs = e.timeStamp;
}

// per packet related processing
void Tracker::processPacket(){
    //find possible combinations of LED positions
    combinationAnalyzer->evaluate();
    Combinations *hypotheses = combinationAnalyzer->getHypotheses();

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        int index = hypotheses->get(0)->get(i);
        Particle *p = particleFilters[i]->get(index);
        widget->updateMaxWeightParticle(i,p);
    }

    combinationAnalyzer->reset();
}

Transition Tracker::getTransition(Event e){
    //Get last event at same position and overwrite with new event.
    Event last = latestEvents->get(e.x,e.y);
    latestEvents->insert(e.x,e.y,e);
    if(last.timeStamp == 0)
        return Transition(0);
    //If consecutive events differ in type, create and return a transition
    if(last.type != e.type)
        return Transition(e.timeStamp,e.x,e.y,e.type);
    else
        return Transition(0);
}

Interval Tracker::getInterval(Transition t){
    Map<Transition> *transitions = (t.type == 1) ? npTransitions : pnTransitions;
    Transition last = transitions->get(t.x,t.y);
    transitions->insert(t.x,t.y,t);

    if(last.timeStamp == 0)
        return Interval(0);
    else{
        double deltaT = t.timeStamp - last.timeStamp;
        return Interval(t.timeStamp,t.x,t.y,deltaT);
    }
}

void Tracker::stop(){
    exit = true;
}

void Tracker::setWidget(CamWidget *camWidget){
    widget = camWidget;
}

void Tracker::updateCamWidget(Event *e){
    if(widget == 0)
        return;
    widget->updateImage(e);
}

void Tracker::updateWeightWidget(int bufID, FrequencyAccumulator *buf, Maxima *m){
    for(int y = 0; y < DVS_RES;y++){
        for(int x = 0; x < DVS_RES;x++){
            float value = buf->weightMap->get(x,y);
            if(value > 0){
                int grey = int(value/4.0);
                if(grey > 255)
                    grey = 255;
                if(grey > 0){
                    widget->updateImage(x,y,grey);
                }
            }
        }
    }
}

void Tracker::run(){
    while(!exit){
        if(packetBuffer->hasNewData()){
            EventPacket *packet = 0;
            while((packet = packetBuffer->getNextReadable()) != 0){
                for(int i = 0; i < packet->size(); i++){
                    // do not process if special event
                    Event *e = packet->get(i);
                    if(e->isSpecial())
                        return;

                    //process events here
                    //updateCamWidget(e);
                    processEvent(*e);
                }
                processPacket();
            }
        }
        else
            msleep(1);
    }
}

#include "eventprocessor.h"
#include "stdio.h"
#include <math.h>

//parameters from thesis
#define ASSIGN_SHARPNESS 0.7f  // of the Boltzman function
#define ASSIGN_PROB 0.6f
#define EVENT_ASSIGN_CHANCE 10.0f //100 with squared distance
#define EVENT_ASSIGN_THRESHOLD 1.4f //1.4f
#define ACTIVITY_THRESHOLD 1.0f //1.0f
#define MIN_CONVERSION_LIFETIME 1000000 // minimal lifetime for a candidate cluster to become a feature cluster
#define MIN_CANDIDATE_LIFETIME 100000   // minimum lifetime before deletion
#define MERGE_THRESHOLD 10.0f   //100 with squared distance
#define SPATIAL_SHARPNESS 2.0f //2.0
#define TEMPORAL_SHARPNESS 500.0f
#define CLUSTER_ASSIGN_THRESHOLD 0.4f

//other
#define DVS_RES 128

EventProcessor::EventProcessor(){
    filter = new Filter();

    camWidget = new CamWidget(&clusters,filter->getHistogram());
    camWidget->show();
    exit = false;
}

EventProcessor::~EventProcessor(){
    delete camWidget;
    delete filter;
}

void EventProcessor::processEvent(Event e){
    // do not process if special event
    if(e.isSpecial())
        return;

    //camWidget->updateImage(&e);

    //filter background activity
    Event* candidates = filter->labelingFilter(e);
    for(int i = 0; i < filter->availableEvents(); i++){
        camWidget->updateImage(&candidates[i]); //graphical output
        assignToCluster(candidates[i]); //assign new events to clusters
    }

    //update all clusters with latest timestamp (for lifetime and activity measurements)
    for(unsigned int i = 0; i < clusters.size();i++){
        clusters[i]->update(e.timeStamp);
    }

    maintainClusters();
}

float EventProcessor::distance(Event *e, Cluster *c){
    return sqrt(pow(float(e->posX-c->posX),2) + pow(float(e->posY-c->posY),2));
}

float EventProcessor::distance(Cluster *c1, Cluster *c2){
    return sqrt(pow(float(c1->posX-c2->posX),2) + pow(float(c1->posY-c2->posY),2));
}

float EventProcessor::squaredDistance(Cluster *c1, Cluster *c2){
    return pow(float(c1->posX-c2->posX),2) + pow(float(c1->posY-c2->posY),2);
}

float EventProcessor::getBoltzmanWeight(Event *e, Cluster *c){
    return exp(-ASSIGN_SHARPNESS*distance(e,c));
}

float EventProcessor::getSpatioTemporalCost(Event *e, Cluster *c){
    float spatial = boundaryCost(e,c);
    float temporal = temporalCost(e,c);

    float pSpatial = 1 - cumulativeDistribution(1/SPATIAL_SHARPNESS,spatial);
    float pTemporal = 1 - cumulativeDistribution(1/TEMPORAL_SHARPNESS,temporal);

    return -log(pSpatial * pTemporal);
}

float EventProcessor::boundaryCost(Event *e, Cluster *c){
    float dx = 0;
    if(!(c->posX + c->contourX >= e->posX && c->posX - c->contourX <= e->posX)){
        dx = std::min(pow(c->posX + c->contourX - e->posX,2),(pow(c->posX - c->contourX - e->posX,2)));
    }

    float dy = 0;
    if(!(c->posY + c->contourY >= e->posY && c->posY - c->contourY <= e->posY)){
        dy = std::min(pow(c->posY + c->contourY - e->posY,2),(pow(c->posY - c->contourY - e->posY,2)));
    }
    return sqrt(dx + dy);
}

float EventProcessor::temporalCost(Event *e, Cluster *c){
    if(!c->assigned){
        return 0.5f;
    }
    else{
        int relative = (e->timeStamp - c->transitionHistory->phase) % c->transitionHistory->period;
        if(relative < 0){
            relative = c->transitionHistory->phase - relative;
        }

        int onDiff = relative;
        int offDiff = abs(c->transitionHistory->period/2 - relative);
        if(e->polarity == 1)
            return onDiff;
        else
            return offDiff;
    }
}

float EventProcessor::cumulativeDistribution(float l, float x){
    return (x < 0) ? 0 : (1-exp(-x*l));
}

void EventProcessor::assignToCluster(Event e){
    float sum = 0;
    float lowest = 100000.0f;
    int clusterIndex = 0;
    for(unsigned int i = 0; i < clusters.size(); i++){
        if(distance(&e,clusters[i]) < EVENT_ASSIGN_CHANCE){
            float cost = getSpatioTemporalCost(&e,clusters[i]);
            if(cost < EVENT_ASSIGN_THRESHOLD){
                if(cost < lowest){
                    lowest = cost;
                    clusterIndex = i;
                }
                sum += exp(cost*-ASSIGN_SHARPNESS); //Boltzmann weight sum
            }
        }
    }
    //normalize
    if(lowest < EVENT_ASSIGN_THRESHOLD){
        float p = exp(lowest*-ASSIGN_SHARPNESS)/sum;  //probability
        if(p > ASSIGN_PROB){
            clusters[clusterIndex]->addEvent(e);
        }
        return;
    }
    // create a new cluster
    Cluster  *c = new Cluster();
    c->addEvent(e);
    clusters.push_back(c);
}

void EventProcessor::maintainClusters(){
    //merge clusters close by
    for(unsigned int i = 0; i < clusters.size(); i++){
        for(unsigned int j = i+1; j < clusters.size(); j++){
            if(distance(clusters[i],clusters[j]) < MERGE_THRESHOLD){
                if(!clusters[i]->isCandidate() && !clusters[j]->isCandidate())
                    break;
                else if(clusters[i]->isCandidate() && clusters[j]->isCandidate()){
                    if(clusters[i]->lifeTime < clusters[j]->lifeTime){
                        delete clusters[i];
                        clusters.erase(clusters.begin()+i);
                        i--;
                    }
                    else{
                        delete clusters[j];
                        clusters.erase(clusters.begin()+j);
                        j--;
                    }
                }
                else if(clusters[i]->isCandidate()){
                    delete clusters[i];
                    clusters.erase(clusters.begin()+i);
                    i--;
                }
                else{
                    delete clusters[j];
                    clusters.erase(clusters.begin()+j);
                    j--;
                }
            }
        }
    }

    //convert candidates to feature clusters
    for(unsigned int i = 0; i < clusters.size();i++){ // TODO: allow only max number of clusters!!
        //printf("lifetime: %d  \r",clusters[i]->lifeTime);
        if(clusters[i]->lifeTime > MIN_CONVERSION_LIFETIME && clusters[i]->isCandidate()){
            clusters[i]->convert();
        }
    }

    //delete inactive/old clusters
    for(unsigned int i = 0; i < clusters.size();i++){
        float activity = clusters[i]->getActivity();
        if(clusters[i]->lifeTime > MIN_CANDIDATE_LIFETIME &&  activity < ACTIVITY_THRESHOLD){
            delete clusters[i];
            clusters.erase(clusters.begin()+i);
            i--;
        }
    }
}

void EventProcessor::stop(){
    exit = true;
}

void EventProcessor::run(){
    while(!exit){
        int size;
        if((size = getEventBuffer()->available()) > 0){
            Event *e;
            while((e = getEventBuffer()->getNext()) != 0){
                // do not process if special event
                if(e->isSpecial())
                    return;

                //                camWidget->updateImage(e);
                //                assignToCluster(*e);

                //                filter background activity
                Event* candidates = filter->labelingFilter(*e);
                for(int i = 0; i < filter->availableEvents(); i++){
                    camWidget->updateImage(&candidates[i]); //graphical output
                    assignToCluster(candidates[i]); //assign new events to clusters
                }
            }
            //update all clusters with latest timestamp (for lifetime and activity measurements)
            int timeStamp = getEventBuffer()->latest()->timeStamp;
            for(unsigned int i = 0; i < clusters.size();i++){
                clusters[i]->update(timeStamp);
            }

            maintainClusters();
        }
        else
            msleep(1);
    }
}

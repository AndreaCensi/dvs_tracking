#include "eventprocessor.h"
#include "stdio.h"
#include <math.h>

//parameters from thesis
#define SHARPNESS 0.7   // of the boltman function
#define ASSIGN_PROB 0.6
//other
#define DVS_RES 128
#define MAX_T_DIFF 50 //usec
#define ACTIVITY_THRESHOLD 5.0
#define FEATURE_LIFETIME 10000

EventProcessor::EventProcessor(){
    img = new QImage(DVS_RES,DVS_RES,QImage::Format_RGB32);

    int length = DVS_RES*DVS_RES;
    onMap = new Event*[length];
    offMap = new Event*[length];
    for(int i = 0; i < length;i++){
        onMap[i] = 0;
        offMap[i] = 0;
    }
    clusterCandidates.reserve(9); // adapt number to filter size
    candidateClusters.reserve(8);
}

EventProcessor::~EventProcessor(){
    delete img;
    delete onMap;
    delete offMap;
}

void EventProcessor::processEvent(Event *e){
    //filter background activity
    std::vector<Event*> candidates = labelingFilter(e);
    for(unsigned int i = 0; i < candidates.size(); i++){
        updateImage(candidates[i]); //graphical output
        assignToCluster(candidates[i]); //assign new events to cluster
    }

    //update all clusters with latest timestamp (for liftime and activity measurements)
    if(!candidates.empty()){
        for(int i = 0; i < clusters.size();i++){
            clusters[i].updateTS(e->timeStamp);
        }
    }

    //merge clusters close by

    //convert candidates to feature clusters
    for(unsigned int i = 0; i < clusters.size();i++){
        if(clusters[i].lifeTime < FEATURE_LIFETIME){
            clusters[i].candidate = false;
        }
    }

    //delete inactive/old clusters
    for(unsigned int i = 0; i < clusters.size();i++){
        if(clusters[i].getActivity() < ACTIVITY_THRESHOLD){
            clusters.erase(clusters.begin()+i);
        }
    }

    //updateImage(e);
    //delete e;
}

QImage* EventProcessor::getImage(){
    return img;
}

void EventProcessor::updateImage(Event *e){
    int x = 127-e->posX;
    int y = 127-e->posY;
    int type = e->polarity;
    QColor color;
    if(type == 1)
        color = Qt::red;
    else
        color = Qt::blue;
    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

float EventProcessor::distance(Event *e, Cluster *c){
    return sqrt(pow(float(e->posX-c->posX),2) + pow(float(e->posY-c->posY),2));
}

float EventProcessor::distance(Event *e1, Event *e2){
    return sqrt(pow(float(e1->posX-e2->posX),2) + pow(float(e1->posY-e2->posY),2));
}

float EventProcessor::distance(Cluster *c1, Cluster *c2){
    return sqrt(pow(float(c1->posX-c2->posX),2) + pow(float(c1->posY-c2->posY),2));
}

float EventProcessor::getBoltzmanWeight(Event *e, Cluster *c){
    return exp(-SHARPNESS*distance(e,c));
}

void EventProcessor::updateMap(Event *e){
    //assign event to on/off map
    Event **map;
    Event *tmp;
    int x = 127-e->posX;
    int y = 127-e->posY;

    if(e->polarity == 1){
        map = onMap;
    }
    else{
        map = offMap;
    }

    tmp = map[x+DVS_RES*y];
    if(tmp != 0 && tmp->assigned == false){ //delete previous event if it's not assigned to any cluster
        //TODO: check TDiff for one pixel LED flashing?
        delete tmp;
    }
    map[x+DVS_RES*y] = e;
}

void EventProcessor::assignToCluster(Event *e){
    if(clusters.empty()){
        Cluster *c = new Cluster();
        c->addEvent(e);
        clusters.push_back(c);
    }
    else{
        // get Boltzman weights
        float sumBoltz = 0;
        float *weights = new float[clusters.size()];
        for(unsigned int i = 0; i < clusters.size(); i++){
            weights[i] = getBoltzmanWeight(e,clusters[i]);
            sumBoltz += weights[i];
        }
        //normalize
        int mostProbIndex = 0;
        for(unsigned int i = 0; i < clusters.size();i++){
            weights[i] = weights[i]/sumBoltz;
            if(weights[i] > weights[mostProbIndex]){
                mostProbIndex = i;
            }
        }
        if(weights[mostProbIndex] > ASSIGN_PROB){
            clusters[mostProbIndex]->addEvent(e);
        }
        else{
            Cluster  *c = new Cluster();
            c->addEvent(e);
            clusters.push_back(c);
        }
        delete [] weights;
    }
}


void EventProcessor::mapAssign(Event *e){
    Event **map;
    Event *tmp;
    int x = 127-e->posX;
    int y = 127-e->posY;

    if(e->polarity == 1){
        map = onMap;
    }
    else{
        map = offMap;
    }
    //Check neighbouring events, only if inside boundary
    int range = 1;  //determines size of filter kernel
    int tDiff = 0;
    if(x > 0 && x < DVS_RES-1 && y > 0 && y < DVS_RES-1){
        for(int u = x-range; u < x+range; u++){
            for(int v = y-range; v < y+range; v++){
                tmp =  map[u+DVS_RES*v];
                if(tmp!=e){
                    tDiff = e->timeStamp - tmp->timeStamp;
                    tDiff = abs(tDiff);
                    if(tDiff < MAX_T_DIFF){ //if tDiff with neighbouring event small, cluster
                        if(tmp->cluster == 0){
                            clusterCandidates.push_back(tmp);
                        }
                        else{
                            candidateClusters.push_back(tmp->cluster);
                        }
                    }
                }
            }
        }
    }
    if(clusterCandidates.size() > 0){
        clusterCandidates.push_back(e); //add current event to clusterCandidates if neighbours found
        //check if any events are already assigned to a cluster
        if(candidateClusters.size() == 1){  //if one cluster found, assign events to existing cluster
            for(unsigned int i = 0; i < clusterCandidates.size();i++){
                clusterCandidates[i]->cluster = candidateClusters[0];
                candidateClusters[0]->addEvent(clusterCandidates[i]);
            }
        }
        else if(candidateClusters.size() > 1){
            int bestIndex = 0;
            float lowestDist = 2*DVS_RES;
            float currentDist = 0;
            for(unsigned int i = 0; i < candidateClusters.size();i++){  // assign current event to nearest cluster
                currentDist = distance(e,candidateClusters[i]);
                if(currentDist < lowestDist){
                    lowestDist = currentDist;
                    bestIndex = i;
                }
            }
            e->cluster = candidateClusters[bestIndex];
            candidateClusters[bestIndex]->addEvent(e);
            for(unsigned int i = 0; i < clusterCandidates.size();i++){  //execute cluster assignment for alles unassigned neighbouring events
                assignToCluster(clusterCandidates[i]);
            }
        }
        else{ //no clusters, make new one
            Cluster *c = new Cluster();
            for(unsigned int i = 0; i < clusterCandidates.size();i++){
                clusterCandidates[i]->cluster = c;
                c->addEvent(clusterCandidates[i]);
            }
            clusters.push_back(c);
        }
    }
}

std::vector<Event*> EventProcessor::labelingFilter(Event *e){

    updateMap(e);   // update filter map

    Event **map = 0;
    Event *tmp = 0;
    int x = 127-e->posX;
    int y = 127-e->posY;

    if(e->polarity == 1){
        map = onMap;
    }
    else{
        map = offMap;
    }

    //Check neighbouring events, only if inside boundary
    std::vector<Event*> candidates;
    candidates.reserve(8);
    int range = 1;  //determines size of filter kernel
    int tDiff = 0;
    if(x > 0 && x < DVS_RES-1 && y > 0 && y < DVS_RES-1){
        for(int u = x-range; u < x+range; u++){
            for(int v = y-range; v < y+range; v++){
                tmp =  map[u+DVS_RES*v];
                if(tmp != 0 && tmp != e){
                    tDiff = e->timeStamp - tmp->timeStamp;
                    tDiff = abs(tDiff);
                    if(tDiff < MAX_T_DIFF && tmp->assigned == false){ //if tDiff with neighbouring event small, cluster
                        tmp->assigned = true; // set flag, that event is assigned and not to be deleted by the filter map
                        candidates.push_back(tmp);
                    }
                }
            }
        }
        if(!candidates.empty())
            candidates.push_back(e);
    }
    return candidates;
}

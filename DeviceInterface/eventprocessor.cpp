#include "eventprocessor.h"
#include "stdio.h"
#include <math.h>

//parameters from thesis
#define SHARPNESS 0.7
#define ASSIGN_PROB 0.6
//other
#define DVS_RES 128
#define MAX_T_DIFF 50

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
    updateImage(e);
    //temporary cleanup
    updateMap(e);
    //assignToCluster(e);
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

//distance with circular boundary
float EventProcessor::distance(Event *e, Cluster *c){
    return sqrt(pow(float(e->posX-c->posX),2) + pow(float(e->posY-c->posY),2));
}

float EventProcessor::distance(Event *e1, Event *e2){
    return sqrt(pow(float(e1->posX-e2->posX),2) + pow(float(e1->posY-e2->posY),2));
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
    if(tmp != 0 && tmp->cluster == 0){ //delete previous event if it's not assigned to any cluster
        //TODO: check TDiff?
        delete tmp;
    }
    map[x+DVS_RES*y] = e;
}

void EventProcessor::assignToCluster(Event *e){
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
    int tDiff = 0;
    if(x > 0 && x < DVS_RES-1 && y > 0 && y < DVS_RES-1){
        for(int u = x-1; u < x+1; u++){
            for(int v = y-1; v < y+1; v++){
                tmp =  map[u+DVS_RES*v];
                if(tmp!=e){
                    tDiff = e->timeStamp - tmp->timeStamp;
                    if(abs(tDiff) < MAX_T_DIFF){ //if tDiff with neighbouring event small, cluster
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
            float lowestDist = 256;
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
            clusters.push_front(c);
        }
    }
}

#include "eventprocessor.h"
#include "stdio.h"
#include <math.h>

//parameters from thesis
#define ASSIGN_SHARPNESS 0.7f  // of the Boltzman function
#define ASSIGN_PROB 0.6f
#define EVENT_ASSIGN_CHANCE 10.0f //100 with squared distance
#define EVENT_ASSIGN_THRESHOLD 1.4f //1.4f
#define MAX_T_DIFF 50 //usec
#define ACTIVITY_THRESHOLD 1.0f //1.0f
#define MIN_CONVERSION_LIFETIME 1000000 // minimal lifetime for a candidate cluster to become a feature cluster
#define MIN_CANDIDATE_LIFETIME 100000   // minimum lifetime before deletion
#define MERGE_THRESHOLD 10.0f   //100 with squared distance
#define SPATIAL_SHARPNESS 2.0f
#define TEMPORAL_SHARPNESS 500.0f

//other
#define DVS_RES 128


EventProcessor::EventProcessor(){
    int length = DVS_RES*DVS_RES;
    onMap = new Event[length];
    offMap = new Event[length];
    Event e;
    for(int i = 0; i < length;i++){
        onMap[i] = e;
        offMap[i] = e;
    }

    clusterCandidates.reserve(9); // adapt number to filter size
    candidateClusters.reserve(8);

    camWidget = new CamWidget(&clusters);
    camWidget->show();
}

EventProcessor::~EventProcessor(){
    delete camWidget;
    delete [] onMap;
    delete [] offMap;
}

void EventProcessor::processEvent(Event e){
    // do not process if special event
    if(e.isSpecial())
        return;

    //filter background activity
    std::vector<Event> candidates = labelingFilter(e);
    for(unsigned int i = 0; i < candidates.size(); i++){
        camWidget->updateImage(&candidates[i]); //graphical output
        assignToCluster(candidates[i]); //assign new events to clusters
    }

    //camWidget->updateImage(&e); //graphical output
    assignToCluster(e); //assign new events to clusters

    //update all clusters with latest timestamp (for lifetime and activity measurements)
    for(unsigned int i = 0; i < clusters.size();i++){
        clusters[i]->updateTS(e.timeStamp);
    }

    maintainClusters();
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
    if(!c->transitionHistory){ //later: isComputable by a certain criterion
        return 0.5f;
    }
    else{
        int relative = (e->timeStamp - c->transitionHistory->phase) % c->transitionHistory->period;
        if(relative < 0){
            relative = c->transitionHistory->phase - relative;
        }
        int min = 100000;

        // find matching transition in transition history
        //        for (int i = -1; i < this.signal.getSize(); i++) {
        //            Transition t = this.signal.getTransition(i);
        //            if (type == t.state) {
        //                int difference = Math.abs(relative - t.time);

        //                if (min > difference) {
        //                    min = difference;
        //                }
        //            }
        //        }

        return min;

        return 0;
    }

}

float EventProcessor::cumulativeDistribution(float l, float x){
    return (x < 0) ? 0 : (1-exp(-x*l));
}

void EventProcessor::updateMap(Event e){
    //assign event to on/off map
    Event *map;

    int x = 127-e.posX;
    int y = 127-e.posY;

    if(e.polarity == 1){
        map = onMap;
    }
    else{
        map = offMap;
    }

    map[x+DVS_RES*y] = e;
}

void EventProcessor::assignToCluster(Event e){
    if(clusters.empty()){
        Cluster *c = new Cluster();
        c->addEvent(e);
        clusters.push_back(c);
    }
    else{
        float sum = 0;
        float lowest = 10000.0f;
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
        }
        else{
            Cluster  *c = new Cluster();
            c->addEvent(e);
            clusters.push_back(c);
        }
    }
}

//void EventProcessor::assignToCluster(Event e){
//    if(clusters.empty()){
//        Cluster *c = new Cluster();
//        c->addEvent(e);
//        clusters.push_back(c);
//    }
//    else{
//        float lowest = 10000.0f;
//        float cost = 0;
//        int clusterIndex = 0;
//        for(unsigned int i = 0; i < clusters.size(); i++){
//            cost=distance(&e,clusters[i]);
//            if(cost < CLUSTER_ASSIGN_CHANCE){
//                if(cost < lowest){
//                    lowest = cost;
//                    clusterIndex = i;
//                }
//            }
//        }
//        if(cost < CLUSTER_ASSIGN_THRESHOLD){
//            clusters[clusterIndex]->addEvent(e);
//        }
//        else{
//            Cluster  *c = new Cluster();
//            c->addEvent(e);
//            clusters.push_back(c);
//        }
//    }
//}

std::vector<Event> EventProcessor::labelingFilter(Event e){
    updateMap(e);   // update filter map

    Event *map = 0;
    Event *tmp = 0;
    int x = 127-e.posX;
    int y = 127-e.posY;

    if(e.polarity == 1){
        map = onMap;
    }
    else{
        map = offMap;
    }

    //Check neighbouring events, only if inside boundary
    std::vector<Event> candidates;
    candidates.reserve(8);
    int range = 1;  //determines size of filter kernel
    int tDiff = 0;
    if(x >= range && x < DVS_RES-range && y >= range && y < DVS_RES-range){
        for(int u = x-range; u < x+range; u++){
            for(int v = y-range; v < y+range; v++){
                tmp =  &map[u+DVS_RES*v];
                if(!(u == x && v == y)){
                    tDiff = e.timeStamp - tmp->timeStamp;
                    tDiff = abs(tDiff);
                    if(tDiff < MAX_T_DIFF && tmp->assigned == false){ //if tDiff with neighbouring event small, cluster
                        tmp->assigned = true; // set flag, that event is assigned and not to be deleted by the filter map
                        candidates.push_back(*tmp);
                    }
                }
            }
        }
        if(!candidates.empty()){
            e.assigned = true;
            candidates.push_back(e);
        }
    }
    return candidates;
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
            clusters[i]->candidate = false;
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

//void EventProcessor::mapAssign(Event *e){
//    Event **map;
//    Event *tmp;
//    int x = 127-e->posX;
//    int y = 127-e->posY;

//    if(e->polarity == 1){
//        map = onMap;
//    }
//    else{
//        map = offMap;
//    }
//    //Check neighbouring events, only if inside boundary
//    int range = 1;  //determines size of filter kernel
//    int tDiff = 0;
//    if(x > 0 && x < DVS_RES-1 && y > 0 && y < DVS_RES-1){
//        for(int u = x-range; u < x+range; u++){
//            for(int v = y-range; v < y+range; v++){
//                tmp =  map[u+DVS_RES*v];
//                if(tmp!=e){
//                    tDiff = e->timeStamp - tmp->timeStamp;
//                    tDiff = abs(tDiff);
//                    if(tDiff < MAX_T_DIFF){ //if tDiff with neighbouring event small, cluster
//                        if(tmp->cluster == 0){
//                            clusterCandidates.push_back(tmp);
//                        }
//                        else{
//                            candidateClusters.push_back(tmp->cluster);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    if(clusterCandidates.size() > 0){
//        clusterCandidates.push_back(e); //add current event to clusterCandidates if neighbours found
//        //check if any events are already assigned to a cluster
//        if(candidateClusters.size() == 1){  //if one cluster found, assign events to existing cluster
//            for(unsigned int i = 0; i < clusterCandidates.size();i++){
//                clusterCandidates[i]->cluster = candidateClusters[0];
//                candidateClusters[0]->addEvent(clusterCandidates[i]);
//            }
//        }
//        else if(candidateClusters.size() > 1){
//            int bestIndex = 0;
//            float lowestDist = 2*DVS_RES;
//            float currentDist = 0;
//            for(unsigned int i = 0; i < candidateClusters.size();i++){  // assign current event to nearest cluster
//                currentDist = distance(e,candidateClusters[i]);
//                if(currentDist < lowestDist){
//                    lowestDist = currentDist;
//                    bestIndex = i;
//                }
//            }
//            e->cluster = candidateClusters[bestIndex];
//            candidateClusters[bestIndex]->addEvent(e);
//            for(unsigned int i = 0; i < clusterCandidates.size();i++){  //execute cluster assignment for alles unassigned neighbouring events
//                assignToCluster(clusterCandidates[i]);
//            }
//        }
//        else{ //no clusters, make new one
//            Cluster *c = new Cluster();
//            for(unsigned int i = 0; i < clusterCandidates.size();i++){
//                clusterCandidates[i]->cluster = c;
//                c->addEvent(clusterCandidates[i]);
//            }
//            clusters.push_back(c);
//        }
//    }
//}

#include "filter.h"
#include <math.h>

#define NEIGHBORHOOD 9  //neighborhood of filter kernel
#define RADIUS 1    //range of filter kernel

Filter::Filter(int res)
{
    resolution = res;
    int length = res*res;
    onMap = new Event[length];
    offMap = new Event[length];

    candidates = new Event[NEIGHBORHOOD];

    Event e;
    for(int i = 0; i < length;i++){ //fill with dummy events
        onMap[i] = e;
        offMap[i] = e;
    }
}

Filter::~Filter(){
    delete [] onMap;
    delete [] offMap;
    delete [] candidates;
}

void Filter::reset(){
    empty = true;
    index = 0;
}

void Filter::updateMap(Event e){
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

    map[x+resolution*y] = e;
}

Event* Filter::labelingFilter(Event e, int maxTDiff){
    reset();    //reset vars
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
    int range = RADIUS;  //determines size of filter kernel
    int tDiff = 0;
    if(x >= range && x < resolution-range && y >= range && y < resolution-range){
        for(int u = x-range; u < x+range; u++){
            for(int v = y-range; v < y+range; v++){
                tmp = &map[u+resolution*v];
                if(!(u == x && v == y)){
                    tDiff = e.timeStamp - tmp->timeStamp;
                    tDiff = abs(tDiff);
                    if(tDiff < maxTDiff && tmp->assigned == false){ //if tDiff with neighbouring event small, cluster
                        tmp->assigned = true; // set flag, that event is assigned and not to be deleted by the filter map
                        candidates[index] = *tmp;
                        index++;
                        empty = false;
                    }
                }
            }
        }
        if(!empty){
            e.assigned = true;
            candidates[index] = e;
            index++;
        }
    }
    return candidates;
}

int Filter::size(){
    return index;
}

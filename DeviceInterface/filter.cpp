#include "filter.h"
#include <math.h>

#define NEIGHBORHOOD 9  //neighborhood of filter kernel
#define RADIUS 1    //range of filter kernel
#define ISI_RES 256
#define MAX_DT  3000
#define MIN_DT  200
#define MAX_COUNT 5000
#define AGEING_INTERVAL 1000
#define THRESHOLD 2000
#define MAX_T_DIFF 50 //usec for temporal correlation of events

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

    isiHistogram = new int[ISI_RES];
    for(int i = 0; i < ISI_RES;i++)
        isiHistogram[i] = 0;

    lastAgeingTS = 0;
}

Filter::~Filter(){
    delete [] onMap;
    delete [] offMap;
    delete [] candidates;
    delete [] isiHistogram;
}

void Filter::reset(){
    empty = true;
    index = 0;
}

Event* Filter::labelingFilter(Event e){
    reset();    //reset vars

    int x = 127-e.posX;
    int y = 127-e.posY;
    Event *map = 0;
    Event *tmp = 0;
    if(e.polarity == 1){
        map = onMap;
    }
    else{
        map = offMap;
    }

    // update histogram
    int interval = e.timeStamp - map[x+resolution*y].timeStamp;
    int age = e.timeStamp - lastAgeingTS;

    bool accept = false;

    if(interval > MIN_DT && interval < MAX_DT){
        int histIndex = (interval-MIN_DT)/((MAX_DT-MIN_DT)/ISI_RES);
        if(isiHistogram[histIndex] < MAX_COUNT){
            isiHistogram[histIndex]++;
        }
        if(isiHistogram[histIndex] > THRESHOLD)
            accept = true;
    }
    else if(age > AGEING_INTERVAL){
        lastAgeingTS = e.timeStamp;
        for(int i = 0; i < ISI_RES; i++){
            isiHistogram[i] -= age/AGEING_INTERVAL;
            if(isiHistogram[i] < 0)
                isiHistogram[i] = 0;
        }
    }

    // update map
    map[x+resolution*y] = e;

    if(true){
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
                        if(tDiff < MAX_T_DIFF && tmp->assigned == false){ //if tDiff with neighbouring event small, cluster
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
    }
    return candidates;

}

int Filter::availableEvents(){
    return index;
}

int* Filter::getHistogram(){
    return isiHistogram;
}

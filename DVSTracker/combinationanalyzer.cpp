#include "combinationanalyzer.h"

CombinationAnalyzer::CombinationAnalyzer(int numLEDs, float minimumDistance)
{
    numTracks = numLEDs;
    minSquaredDistance = minimumDistance*minimumDistance;
}

bool CombinationAnalyzer::isReady(){
    return false;
}

bool CombinationAnalyzer::containsNeighbour(int depth, CombinationChoice *c, ParticleFilter **pFilters){
    if(depth > 0){
        Particle *p1 = pFilters[depth]->get(c->get(depth));
        Particle *p2 = pFilters[depth-1]->get(c->get(depth-1));
        float squaredDistance = (pow(float(p1->x-p2->x),2.0f) + pow(float(p1->y-p2->y),2.0f));

        if(squaredDistance > minSquaredDistance)
            return false;
        else
            return true;

    }
    else
        return false;
}

float CombinationAnalyzer::getLikelihood(CombinationChoice *c, int size){
    return 0;
}

void CombinationAnalyzer::analyze(CombinationChoice last, int depth,
                                  ParticleFilter **pFilters){
    //if all branches have been traversed return
    if(depth >= numTracks){
        for(int i = 0; i < last.size();i++)
            printf("%d ",last.get(i));
        printf("\n");
        return;
    }

    //    if(found.size() > numHypothesis)
    //        currentThreshold = found.getWorst();
    //    else
    //        currentThreshold = 0;

    //if(getLikelihood(CombinationChoice *c) > currentThreshold)

    ParticleFilter *pf = pFilters[depth];

    // branch
    for(int i = 0; i < pf->size();i++){
        last.set(depth,i);
        if(!containsNeighbour(depth,&last,pFilters)){   //only branch if current and previous depth particle are not neighbouring
            analyze(last,depth+1,pFilters);
        }
    }
}

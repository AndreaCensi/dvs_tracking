#include "combinationanalyzer.h"

CombinationAnalyzer::CombinationAnalyzer(int numLEDs, float minimumDistance)
{
    numTracks = numLEDs;
    minDistance = minimumDistance;
}

bool CombinationAnalyzer::isReady(){
    return false;
}

bool CombinationAnalyzer::neighbour(Particle *p1, Particle *p2){
    return false;
}

float CombinationAnalyzer::getLikelihood(CombinationChoice *c, int size){
    return 0;
}

void CombinationAnalyzer::analyze(CombinationChoice last, int depth,
                                  ParticleFilter **pFilters){
    //    if(last->get(numLEDs-1) == pf[numLEDs-1]->size){
    //        //found.add(last)
    //        return /*found*/;
    //    }
    //    if(found.size() > numHypothesis)
    //        currentThreshold = found.getWorst();
    //    else
    //        currentThreshold = 0;

    //    currentChoice = last->increment();

    //if(getLikelihood(CombinationChoice *c) > currentThreshold)

    //    Particle *p1 = pFilters[depth]->get(last.get(depth));
    //    Particle *p2 = pFilters[depth+1]->get(last.get(depth+1));

    //    if(neighbour(p1,p2))
    //        return;

    if(depth >= numTracks){
        printf("depth %d: ", depth);
        for(int i = 0; i < last.size();i++)
            printf("%d ",last.get(i));
        printf("\n");
        return;
    }

    ParticleFilter *pf = pFilters[depth];

    for(int i = 0; i < pf->size();i++){
        last.set(depth,i);
        analyze(last,depth+1,pFilters);
    }
}

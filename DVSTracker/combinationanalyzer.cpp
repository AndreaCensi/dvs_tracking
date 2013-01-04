#include "combinationanalyzer.h"

CombinationAnalyzer::CombinationAnalyzer(int numLEDs, float minimumDistance, int numOfHypothesis)
{
    numTracks = numLEDs;
    minSquaredDistance = minimumDistance*minimumDistance;
    numHypothesis = numOfHypothesis;
    found = new Combinations(numHypothesis*2, numTracks);
    reset();
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

float CombinationAnalyzer::getLikelihood(ParticleFilter **pFilters, CombinationChoice *c){
    float likelihood = 1;
    for(int i = 0; i < c->size(); i++)
        likelihood *= (float)pFilters[i]->get(c->get(i))->weight;
    return likelihood;
}

void CombinationAnalyzer::analyze(CombinationChoice choice, int depth,
                                  ParticleFilter **pFilters){
    //if all branches have been traversed return
    if(depth >= numTracks){
        found->add(&choice);
        for(int i = 0; i < choice.size();i++)
            printf("%d ",choice.get(i));
        printf(", l: %f\n",choice.score);
        return;
    }

    if(found->size() > numHypothesis)
        threshold = found->getLowestScore();
    else
        threshold = 0;

    ParticleFilter *pf = pFilters[depth];

    // branch
    for(int i = 0; i < pf->size();i++){
        choice.set(depth,i);
        if(!containsNeighbour(depth,&choice,pFilters)){   //only branch if current and previous depth particle are not neighbouring
            choice.score = getLikelihood(pFilters, &choice);
            if(choice.score > threshold){
                analyze(choice,depth+1,pFilters);
            }
        }
    }
}

Combinations* CombinationAnalyzer::getHypotheses(){
    return found;
}

void CombinationAnalyzer::reset(){
    found->reset();
    threshold = 0;
}

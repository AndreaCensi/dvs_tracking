#include "combinationanalyzer.h"

#define MISS_PROB 500.0f

CombinationAnalyzer::CombinationAnalyzer(ParticleFilter **pFilters, int numLEDs, float minimumDistance, int numOfHypothesis)
{
    particleFilters = pFilters;
    numTracks = numLEDs;
    minSquaredDistance = minimumDistance*minimumDistance;
    numHypothesis = numOfHypothesis;
    found = new Combinations(numHypothesis*2, numTracks);
    reset();
}

void CombinationAnalyzer::evaluate(){
    for(int i = 0; i < numTracks; i++)
        particleFilters[i]->sortParticles();
    CombinationChoice c(numTracks);
    analyze(c,0);
}

bool CombinationAnalyzer::containsNeighbour(int depth, CombinationChoice *c){
    if(depth > 0){
        int iP1 = c->get(depth);
        int iP2 = c->get(depth-1);
        if(iP1 == particleFilters[depth]->size() || iP2 == particleFilters[depth-1]->size())
            return false;
        Particle *p1 = particleFilters[depth]->get(iP1);
        Particle *p2 = particleFilters[depth-1]->get(iP2);
        float squaredDistance = (pow(float(p1->x-p2->x),2.0f) + pow(float(p1->y-p2->y),2.0f));

        if(squaredDistance > minSquaredDistance)
            return false;
        else
            return true;

    }
    else
        return false;
}

float CombinationAnalyzer::getLikelihood(CombinationChoice *c){
    float likelihood = 1;
    for(int i = 0; i < c->size(); i++){
        if(c->get(i) == (particleFilters[i]->size()) )
            likelihood *= MISS_PROB;
        else
            likelihood *= (float)particleFilters[i]->get(c->get(i))->weight;
    }
    return likelihood;
}

void CombinationAnalyzer::analyze(CombinationChoice choice, int depth){
    //if all branches have been traversed return
    if(depth >= numTracks){
        found->add(&choice);
        return;
    }

    if(found->size() > numHypothesis)
        threshold = found->getLowestScore();
    else
        threshold = 0;

    ParticleFilter *pf = particleFilters[depth];

    // branch
    for(int i = 0; i < pf->size()+1;i++){
        choice.set(depth,i);
        if(!containsNeighbour(depth,&choice)){   //only branch if current and previous depth particle are not neighbouring
            choice.score = getLikelihood(&choice);
            if(choice.score > threshold){
                analyze(choice,depth+1);
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

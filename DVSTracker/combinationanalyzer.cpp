#include "combinationanalyzer.h"

#define DEPTH_LIMIT 8

CombinationAnalyzer::CombinationAnalyzer(ParticleFilter **pFilters, int numLEDs, float minimumDistance, int numOfHypothesis)
{
    particleFilters = pFilters;
    numTracks = numLEDs;
    minSquaredDistance = minimumDistance*minimumDistance;
    numHypothesis = numOfHypothesis;
    found = new Combinations(numHypothesis, numTracks);
    reset();

    counter = 0;
}

void CombinationAnalyzer::evaluate(){
    //sort particles descending by weight
    for(int i = 0; i < numTracks; i++)
        particleFilters[i]->sortParticles();
    // find the most likely combinations
    int level = 0;
    CombinationChoice c(numTracks);
    //analyze(c,0,0,particleFilters[0]->size()+1);

    // expand tree with depth limitation
    while(level < particleFilters[0]->size()){
        for(int i = 0; i < c.size(); i++)
            c.set(i,level);
        analyze(c,0,level,DEPTH_LIMIT);
        level++;
    }
}

bool CombinationAnalyzer::containsNeighbour(CombinationChoice *c, int branch){
    Particle *p1 = particleFilters[branch]->get(c->get(branch));
    for(int i = 0; i < branch;i++){
        Particle *p2 = particleFilters[i]->get(c->get(i));

        float squaredDistance = (pow(float(p1->x-p2->x),2.0f) + pow(float(p1->y-p2->y),2.0f));
        if(squaredDistance < minSquaredDistance)
            return true;
    }
    return false;
}

float CombinationAnalyzer::getLikelihood(CombinationChoice *c){
    float likelihood = 1;
    for(int i = 0; i < c->size(); i++){
        likelihood *= (float)particleFilters[i]->get(c->get(i))->weight;
    }
    return likelihood;
}

// recursive function to traverse the different combinations
void CombinationAnalyzer::analyze(CombinationChoice choice, int branch, int level, int depthLimit){
    if(branch >= numTracks){
        counter++;
        found->add(&choice);
        return;
    }

    if(found->size() > numHypothesis)
        threshold = found->getWorstScore();
    else
        threshold = 0;

    ParticleFilter *pf = particleFilters[branch];

    // branch
    for(int i = level; i < level+depthLimit;i++){
        if(i >= pf->size())
            return;
        choice.set(branch,i);
        if(!containsNeighbour(&choice,branch)){   //only branch if no particles in branch are neighbouring
            choice.score = getLikelihood(&choice);
            if(choice.score > threshold){
                analyze(choice,branch+1,level,depthLimit);
            }
        }
    }
}

Combinations* CombinationAnalyzer::getHypotheses(){
    return found;
}

CombinationChoice* CombinationAnalyzer::getBestHypothesis(){
    return found->getBestCombination();
}

void CombinationAnalyzer::reset(){
    found->reset();
    threshold = 0;
}

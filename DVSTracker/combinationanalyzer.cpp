#include "combinationanalyzer.h"
#include <cfloat>

#define MISS_PROB 500.0f
#define DEPTH_LIMIT 2

CombinationAnalyzer::CombinationAnalyzer(ParticleFilter **pFilters, int numLEDs, float minimumDistance, int numOfHypothesis)
{
    particleFilters = pFilters;
    numTracks = numLEDs;
    minSquaredDistance = minimumDistance*minimumDistance;
    numHypothesis = numOfHypothesis;
    found = new Combinations(numHypothesis*2, numTracks);
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

    // expand tree with depth limiation
    while(level < particleFilters[0]->size()){
        for(int i = 0; i < c.size(); i++)
            c.set(i,level);
        analyze(c,0,level,DEPTH_LIMIT);
        level++;
    }
}

bool CombinationAnalyzer::containsNeighbour(CombinationChoice *c, int branch){
    if(branch == 0)
        return false;

    int iP1 = c->get(branch);
    for(int i = 0; i < branch;i++){
        int iP2 = c->get(i);

        if(iP1 == particleFilters[branch]->size() || iP2 == particleFilters[i]->size()) // if index refers to miss probability, do not process!
            return false;

        Particle *p1 = particleFilters[branch]->get(iP1);
        Particle *p2 = particleFilters[i]->get(iP2);

        float squaredDistance = (pow(float(p1->x-p2->x),2.0f) + pow(float(p1->y-p2->y),2.0f));
        if(squaredDistance < minSquaredDistance)
            return true;
    }
    return false;
}

float CombinationAnalyzer::getLikelihood(CombinationChoice *c){
    float likelihood = 1;
    for(int i = 0; i < c->size(); i++){
        if(c->get(i) == (particleFilters[i]->size()))
            likelihood *= MISS_PROB;
        else
            likelihood *= (float)particleFilters[i]->get(c->get(i))->uncertainty;
    }
    return likelihood;
}

void CombinationAnalyzer::analyze(CombinationChoice choice, int branch, int level, int depthLimit){
    if(branch >= numTracks){
        counter++;
        found->add(&choice);
        return;
    }

    if(found->size() > numHypothesis)
        threshold = found->getWorstScore();
    else
        threshold = FLT_MAX;

    ParticleFilter *pf = particleFilters[branch];

    // branch
    for(int i = level; i < level+depthLimit;i++){
        if(i > pf->size())
            break;
        choice.set(branch,i);
        if(!containsNeighbour(&choice,branch)){   //only branch if no particles in branch are neighbouring
            choice.score = getLikelihood(&choice);
            if(choice.score < threshold){
                analyze(choice,branch+1,level,depthLimit);
            }
        }
    }
}

Combinations* CombinationAnalyzer::getHypotheses(){
    return found;
}

void CombinationAnalyzer::reset(){
    found->reset();
    threshold = FLT_MAX;
}

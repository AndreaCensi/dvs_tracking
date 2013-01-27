#include "combinationanalyzer.h"

#define DEPTH_LIMIT 8

//! Constructor
/*!
    \param pFilters Pointer to instances of ParticleFilter.
    \param numLEDs The number of LEDs to be tracked.
    \param minimumDistance Minimum allowed distance limit between to different frequencies found.
    \param numOfHypothesis Number of different combinations to be gathered.
*/
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

//! Destructor
CombinationAnalyzer::~CombinationAnalyzer(){
    delete found;
}

//! Evaluate possible LED positions from particle filters
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

//! Checks for neighbouring particles of different frequencies
/*!
    \param c Current choice of possible combintions to be analyzed.
    \param branch The depth index of the current branch. See the recursive function analyze() for more details.
    \return True if a combination contains particles of different frequencies which are too close.
*/
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

//! Calculates the likelihood of a certain combination.
/*!
    \param c Current choice of possible combintions to be analyzed.
    \return The likelihood, i.e. the product of particle weights.
*/
float CombinationAnalyzer::getLikelihood(CombinationChoice *c){
    float likelihood = 1;
    for(int i = 0; i < c->size(); i++){
        likelihood *= (float)particleFilters[i]->get(c->get(i))->weight;
    }
    return likelihood;
}

//!  Recursive function to traverse the different combinations
/*!
    \param choice Current choice of possible combintions to be analyzed.
    \param branch Combinations are expanded per particle filter. This denotes the current particle filter being expanded.
    \param level Level indicates the current index in the sorted particle filter. Used for depth limited search.
    \param depthLimit Depth limited for depth limited search.
*/
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

//! Getter method
/*!
    \return The most likely LED position combinations.
*/
Combinations* CombinationAnalyzer::getHypotheses(){
    return found;
}

//! Getter method
/*!
    \return The most likely LED positions.
*/
CombinationChoice* CombinationAnalyzer::getBestHypothesis(){
    return found->getBestCombination();
}

//! Reset CombinationAnalyzer
void CombinationAnalyzer::reset(){
    found->reset();
    threshold = 0;
}

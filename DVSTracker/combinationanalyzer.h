#ifndef COMBINATIONANALYZER_H
#define COMBINATIONANALYZER_H

#include "particlefilter.h"
#include "combinationchoice.h"
#include "combinations.h"
#include <vector>

class CombinationAnalyzer
{
public:
    CombinationAnalyzer(ParticleFilter **pFilters,int numLEDs, float minimumDistance, int numOfHypothesis);
    void evaluate();
    Combinations* getHypotheses();
    void reset();
    int counter;

private:
    bool containsNeighbour(int depth, CombinationChoice *c);
    float getLikelihood(CombinationChoice *c);
    void analyze(CombinationChoice choice, int depth);

    ParticleFilter **particleFilters;
    Combinations *found;
    int numTracks;
    float threshold;
    float minSquaredDistance;
    int numHypothesis;
};

#endif // COMBINATIONANALYZER_H

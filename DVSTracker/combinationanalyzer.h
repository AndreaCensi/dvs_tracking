#ifndef COMBINATIONANALYZER_H
#define COMBINATIONANALYZER_H

#include "particlefilter.h"
#include "combinationchoice.h"
#include "combinations.h"
#include <vector>

class CombinationAnalyzer
{
public:
    CombinationAnalyzer(int numLEDs, float minimumDistance, int numOfHypothesis);
    void add(int id, Particle **particles); // add particle filter
    bool isReady(); // are all sorted particle filters gathered?
    bool containsNeighbour(int depth, CombinationChoice *c, ParticleFilter **pFilters);
    float getLikelihood(ParticleFilter **pFilters, CombinationChoice *c);
    void analyze(CombinationChoice choice, int depth, ParticleFilter **pFilters);
    Combinations* getHypotheses();
    void reset();

private:
    Combinations *found;
    int numTracks;
    float threshold;
    float minSquaredDistance;
    int numHypothesis;
};

#endif // COMBINATIONANALYZER_H

#ifndef COMBINATIONANALYZER_H
#define COMBINATIONANALYZER_H

#include "particlefilter.h"
#include "combinationchoice.h"
#include <vector>

class CombinationAnalyzer
{
public:
    CombinationAnalyzer(int numLEDs, float minimumDistance);
    void add(int id, Particle **particles); // add particle filter
    bool isReady(); // are all sorted particle filters gathered?
    bool containsNeighbour(int depth, CombinationChoice *c, ParticleFilter **pFilters);
    float getLikelihood(CombinationChoice *c, int size);
    void analyze(CombinationChoice last, int depth, ParticleFilter **pFilters);

private:
    int numTracks;
    float threshold;
    int depth;
    float minSquaredDistance;
};

#endif // COMBINATIONANALYZER_H

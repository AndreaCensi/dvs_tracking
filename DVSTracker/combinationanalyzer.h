#ifndef COMBINATIONANALYZER_H
#define COMBINATIONANALYZER_H

#include "particlefilter.h"
#include "combinationchoice.h"
#include "combinations.h"
#include <vector>

//! A class for evaluating possible tracks
/*!
    Evaluates possible combinations of particles for valid LED tracks.
    \author Jonas Strubel
*/
class CombinationAnalyzer
{
public:
    CombinationAnalyzer(ParticleFilter **pFilters,int numLEDs, float minimumDistance, int numOfHypothesis);
    ~CombinationAnalyzer();
    void evaluate();
    Combinations* getHypotheses();
    CombinationChoice *getBestHypothesis();
    void reset();
    int counter;

private:
    bool containsNeighbour(CombinationChoice *c, int branch);
    float getLikelihood(CombinationChoice *c);
    void analyze(CombinationChoice choice, int branch, int level, int depthLimit);

    ParticleFilter **particleFilters;
    Combinations *found;
    int numTracks;
    float threshold;
    float minSquaredDistance;
    int numHypothesis;
};

#endif // COMBINATIONANALYZER_H

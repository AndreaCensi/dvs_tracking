#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include "combinationchoice.h"

//! A container for combination choices CombinationChoice
/*!
    \author Jonas Strubel
*/
class Combinations
{
public:
    Combinations(int maxNumCombinations, int numTracks);
    ~Combinations();
    void add(CombinationChoice *c);
    CombinationChoice* get(int i);
    float getWorstScore();
    CombinationChoice* getBestCombination();
    int size();
    void reset();

private:
    CombinationChoice **combinations;
    CombinationChoice *best;
    CombinationChoice *worst;

    int length;
    int insertIndex;

};

#endif // COMBINATIONS_H

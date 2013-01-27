#ifndef COMBINATIONCHOICE_H
#define COMBINATIONCHOICE_H

#include "particle.h"

//! A container class for combination choices
/*!
    A possible combination of LED combinations are stored here. The class only stores indices which point to particles in the particle filters.
    \author Jonas Strubel
*/
class CombinationChoice
{
public:
    CombinationChoice(int numTracks);
    int size();
    int get(int i);
    void set(int i, int p);
    void assign(CombinationChoice *c);
    void reset();
    float score;

private:
    int length;
    int *choice;
};

#endif // COMBINATIONCHOICE_H

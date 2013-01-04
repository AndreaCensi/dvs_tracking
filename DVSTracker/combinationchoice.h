#ifndef COMBINATIONCHOICE_H
#define COMBINATIONCHOICE_H

#include "particle.h"

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

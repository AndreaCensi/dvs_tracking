#ifndef COMBINATIONCHOICE_H
#define COMBINATIONCHOICE_H

#include "particle.h"

class CombinationChoice
{
public:
    CombinationChoice(int n);
    int size();
    int get(int i);
    void set(int i, int p);
    void reset();

private:
    int length;
    float score;
    int *choice;
};

#endif // COMBINATIONCHOICE_H

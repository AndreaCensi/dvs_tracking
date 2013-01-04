#include "combinations.h"

Combinations::Combinations(int maxNumCombinations, int numTracks)
{
    length = maxNumCombinations;
    combinations = new CombinationChoice*[length];
    for(int i = 0; i < length; i++)
        combinations[i] = new CombinationChoice(numTracks);
    reset();
}

Combinations::~Combinations(){
    for(int i = 0; i < length; i++)
        delete combinations[i];
    delete [] combinations;
}

void Combinations::add(CombinationChoice *c){
    if(insertIndex < length){
        combinations[insertIndex]->assign(c);
        insertIndex++;
        if(lowestScore > c->score)
            lowestScore = c->score;
    }
}

CombinationChoice* Combinations::get(int i){
    return combinations[i];
}

float Combinations::getLowestScore(){
    return lowestScore;
}

int Combinations::size(){
    return insertIndex;
}

void Combinations::reset(){
    insertIndex = 0;
    lowestScore = 1000000;
}

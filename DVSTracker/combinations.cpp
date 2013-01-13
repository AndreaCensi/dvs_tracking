#include "combinations.h"
#include "cfloat"

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
        if(worstScore < c->score)   //set worst score
            worstScore = c->score;
        if(best->score > c->score)  //set best scoring combination
            best = combinations[insertIndex];
        insertIndex++;
    }
}

CombinationChoice* Combinations::get(int i){
    return combinations[i];
}

float Combinations::getWorstScore(){
    return worstScore;
}

CombinationChoice* Combinations::getBestCombination(){
    return best;
}

int Combinations::size(){
    return insertIndex;
}

void Combinations::reset(){
    insertIndex = 0;
    worstScore = 0;
    best = combinations[0];
}

#include "combinations.h"
#include "cfloat"

//! Constructor
/*!
    \param maxNumCombinations Maximum number of hypothesis to gather.
    \param numTracks Number of LEDs to track.
*/
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
    // if not full insert and update worst and best score
    if(insertIndex < length){
        combinations[insertIndex]->assign(c);
        if(worst->score > c->score)   //set worst score
            worst = combinations[insertIndex];
        if(best->score < c->score)  //set best scoring combination
            best = combinations[insertIndex];
        insertIndex++;
    }
    //replace worst score
    else{
        if(c->score > worst->score)
            worst->assign(c);
        // update best score
        if(best->score < c->score)  //set best scoring combination
            best = worst;

        //search for new worst score after insertion
        for(int i = 0; i < length;i++){
            if(worst->score > combinations[i]->score)   //set worst score
                worst = combinations[i];
        }
    }
}

CombinationChoice* Combinations::get(int i){
    return combinations[i];
}

float Combinations::getWorstScore(){
    return worst->score;
}

CombinationChoice* Combinations::getBestCombination(){
    if (insertIndex > 0)
        return best;
    else
        return 0;
}

int Combinations::size(){
    return insertIndex;
}

void Combinations::reset(){
    insertIndex = 0;
    worst = combinations[0];
    best = combinations[0];
}

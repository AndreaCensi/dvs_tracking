#include "combinationchoice.h"


CombinationChoice::CombinationChoice(int numTracks)
{
    length = numTracks;
    choice = new int[length];
    reset();
}

int CombinationChoice::size(){
    return length;
}

int CombinationChoice::get(int i){
    return choice[i];
}

void CombinationChoice::set(int i, int p){
    choice[i] = p;
}

void CombinationChoice::assign(CombinationChoice *c){
    if(length != c->length)
        return;
    for(int i = 0; i < length; i++)
        choice[i] = c->choice[i];
    score = c->score;
}

void CombinationChoice::reset(){
    for(int i = 0; i < length;i++)
        choice[i] = 0;
    score = 0;
}

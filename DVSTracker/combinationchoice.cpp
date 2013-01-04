#include "combinationchoice.h"

CombinationChoice::CombinationChoice(int n)
{
    length = n;
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

void CombinationChoice::reset(){
    for(int i = 0; i < length;i++)
        choice[i] = 0;
}

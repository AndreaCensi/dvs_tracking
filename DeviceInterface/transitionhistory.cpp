#include "transitionhistory.h"

TransitionHistory::TransitionHistory()
{
    reset();
}

TransitionHistory::~TransitionHistory(){

}

void TransitionHistory::reset(){
    period = 0;
    phase = 0;
}

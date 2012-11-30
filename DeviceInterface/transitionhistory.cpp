#include "transitionhistory.h"

#define NUM_TRANS 256

TransitionHistory::TransitionHistory()
{
    transitionHistory = new RingBuffer<Transition>(NUM_TRANS);
    reset();
}

TransitionHistory::~TransitionHistory(){
    delete transitionHistory;
}

void TransitionHistory::add(Transition t){
    transitionHistory->add(t);
    size++;
}

Transition* TransitionHistory::at(int i){
    return &transitionHistory->at(i);
}

void TransitionHistory::reset(){
    size = 0;
    period = 0;
    phase = 0;
}

bool TransitionHistory::isFull(){
    if(size == NUM_TRANS)
        return true;
    else
        return false;
}

void TransitionHistory::compileTransitions(){
    int diffSum = 0;
    int phaseSum = 0;

    for(int i = 0; i < NUM_TRANS-1;i++){
        diffSum += transitionHistory->at(i+1).timeStamp - transitionHistory->at(i).timeStamp;
    }

    period = 2*(diffSum/(NUM_TRANS-1));

    int onCount = 0;
    for(int i = 0; i < NUM_TRANS;i++){
        if(transitionHistory->at(i).polarity == 1){
            phaseSum += transitionHistory->at(i).timeStamp%period;
            onCount++;
        }
    }
    if(onCount != 0)
        phase = phaseSum/onCount;
}

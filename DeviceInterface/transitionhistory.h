#ifndef TRANSITIONHISTORY_H
#define TRANSITIONHISTORY_H

#include "ringbuffer.h"
#include "transition.h"
#include "event.h"

class TransitionHistory
{
public:
    TransitionHistory();
    ~TransitionHistory();

    void add(Transition t);
    Transition* at(int i);
    bool isFull();
    void compileTransitions();

    int period;
    int phase;

private:
    void reset();
    RingBuffer<Transition> *transitionHistory;
    int size;
};

#endif // TRANSITIONHISTORY_H

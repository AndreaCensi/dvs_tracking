#ifndef TRANSITIONHISTORY_H
#define TRANSITIONHISTORY_H

class TransitionHistory
{
public:
    TransitionHistory();
    ~TransitionHistory();

    void reset();
    int period;
    int phase;

private:
    struct Transition{
        int time;
        int state;
    };
};

#endif // TRANSITIONHISTORY_H

#ifndef TRANSITION_H
#define TRANSITION_H

class Transition
{
public:
    Transition(unsigned int ts = 0, int posX = -1, int posY = -1, int polarityType = -1);
    unsigned int timeStamp;
    int x;
    int y;
    int type;
};

#endif // TRANSITION_H

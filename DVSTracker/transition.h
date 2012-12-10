#ifndef TRANSITION_H
#define TRANSITION_H

class Transition
{
public:
    Transition(double ts = 0, int posX = -1, int posY = -1, int polarityType = -1);
    double timeStamp;
    int x;
    int y;
    int type;
};

#endif // TRANSITION_H

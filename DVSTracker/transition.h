#ifndef TRANSITION_H
#define TRANSITION_H

//! A container for a pixel transition
/*!
    Whenever a pixel changes polarity a transition is generated.
    \author Jonas Strubel
*/
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

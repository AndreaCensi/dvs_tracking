#ifndef EVENT_H
#define EVENT_H

class Event
{
public:
    Event();
    ~Event();
    bool isSpecial();
    int posX;
    int posY;
    unsigned int timeStamp;
    int polarity;
    bool special;
};

#endif // EVENT_H

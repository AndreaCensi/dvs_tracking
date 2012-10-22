#ifndef EVENT_H
#define EVENT_H

class Cluster;  //forward declaration of cluster

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
    Cluster *cluster;
    bool assigned;
};

#endif // EVENT_H

#ifndef EVENTREADER_H
#define EVENTREADER_H

class EventReader
{
public:
    EventReader();
    ~EventReader();
    void processEvent(unsigned char *data);
private:
    unsigned int mileStone;
    struct Event{
        unsigned short xAddr;
        unsigned short yAddr;
        unsigned int timeStamp;
        bool eventType;
    };
};

#endif // EVENTREADER_H

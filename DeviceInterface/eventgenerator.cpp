#include "eventgenerator.h"

EventGenerator::EventGenerator(EventProcessorBase *ep, QObject *parent) : QObject(parent)
{
    eventProcessor = ep;
    //timer.start(1,this);
}

void EventGenerator::timerEvent(QTimerEvent *){
    e[0].posX = 50;
    e[0].posY = 50;

    e[1].posX = 51;
    e[1].posY = 50;

    e[2].posX = 49;
    e[2].posY = 50;

    e[3].posX = 50;
    e[3].posY = 51;

    e[4].posX = 50;
    e[4].posY = 49;

    for(int i = 0; i < 5; i++){
        e[i].timeStamp += 10+i*4;
        e[i].polarity = (!e[i].polarity)&1;
        eventProcessor->processEvent(e[i]);
    }
}

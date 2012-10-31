#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include <QObject>
#include "eventprocessorbase.h"
#include <QBasicTimer>

class EventGenerator : public QObject
{
public:
    EventGenerator(EventProcessorBase *processor, QObject *parent = 0);

protected:
    void timerEvent(QTimerEvent *);

private:
    QBasicTimer timer;
    EventProcessorBase *eventProcessor;
    int counter;
    Event e[5];
};

#endif // EVENTGENERATOR_H

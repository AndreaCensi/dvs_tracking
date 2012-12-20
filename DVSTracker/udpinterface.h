#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include "event.h"
#include "ringbuffer.h"

class UDPInterface : public QObject
{
    Q_OBJECT
public:
    explicit UDPInterface(QObject *parent = 0);
    ~UDPInterface();
    RingBuffer<Event>* getEventBuffer();
    void run();
    
public slots:
    void readPendingDatagrams();

private:
    void readEvents(QByteArray data);

    RingBuffer<Event> *eventBuffer;
    QUdpSocket *socket;
    unsigned int mileStone;

    int counter;
    double lastEventTime,currentEventTime;
};

#endif // UDPINTERFACE_H

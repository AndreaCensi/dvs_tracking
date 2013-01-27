#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include "event.h"
#include "ringbuffer.h"
#include "packetbuffer.h"


//! UDP interface class
/*!
    Used to stream event data from jAER, e.g. for replaying recordings.
    \details For UDP remote output in jAER set the following properties: addressFirstEnabled, includeTimestamps, deselect all others.
    \author Jonas Strubel
*/
class UDPInterface : public QObject
{
    Q_OBJECT
public:
    explicit UDPInterface(QObject *parent = 0);
    ~UDPInterface();
    RingBuffer<Event>* getEventBuffer();
        PacketBuffer* getPacketBuffer();
    void run();
    
public slots:
    void readPendingDatagrams();

private:
    void readEvents(QByteArray data);

    PacketBuffer *packetBuffer;
    RingBuffer<Event> *eventBuffer;

    QUdpSocket *socket;
    unsigned int mileStone;

    //debugging
    int counter;
    double lastEventTime, currentEventTime;
};

#endif // UDPINTERFACE_H

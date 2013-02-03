#include "udpinterface.h"

#define EVENT_FRAME_LENGTH 6
#define NUM_PACKET_BUFS 10

UDPInterface::UDPInterface(QObject *parent) : QObject(parent){
    //eventBuffer = new RingBuffer<Event>(20000);
    packetBuffer = new PacketBuffer(NUM_PACKET_BUFS,65535/EVENT_FRAME_LENGTH);
    mileStone = 0;

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 8991);

    connect(socket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
    counter = 0;
    lastEventTime = 0;
    currentEventTime = 0;
}

UDPInterface::~UDPInterface(){
    delete packetBuffer;
    //delete eventBuffer;
    delete socket;
}

void UDPInterface::readPendingDatagrams(){
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        readEvents(datagram);
    }
}

void UDPInterface::readEvents(QByteArray data){
    int numBytes = data.size();
    if(numBytes%EVENT_FRAME_LENGTH != 0){
        printf("Incorrect data size: %d bytes\n",numBytes);
        return;
    }

    EventPacket *packet = packetBuffer->getNextWritable();
    packet->reset();

    for(int i = 0; i < numBytes; i+=EVENT_FRAME_LENGTH){
        Event event;
        unsigned int rawAddr  = ((data[i+1] & 0xff) | ((data[i] & 0xff) << 8));
        if((rawAddr & (0x8000)) != 0){  //special event
            event.special = true;
        }
        else{
            event.timeStamp = ((data[i+5] & 0xff | ((data[i+4] & 0xff) << 8) |
                                ((data[i+3] & 0xff) << 16) | ((data[i+2] & 0xff) << 24)))/1000000.0;
            event.x = (rawAddr >> 1) & 0x7f;
            event.y = (rawAddr >> 8) & 0x7f;
            event.type = 1 - rawAddr & 1;
        }
        //        printf("ts: %d\n",event.timeStamp);
        packet->add(event);

        //debugging
        currentEventTime = event.timeStamp;

        if(lastEventTime > currentEventTime){
            printf("#Events(udp): %d\n",counter);
            counter = 0;
        }
        else
            counter++;

        lastEventTime = currentEventTime;
    }
}

RingBuffer<Event>* UDPInterface::getEventBuffer(){
    return eventBuffer;
}

PacketBuffer* UDPInterface::getPacketBuffer(){
    return packetBuffer;
}

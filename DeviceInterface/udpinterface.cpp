#include "udpinterface.h"

#define FRAME_LENGTH 6

UDPInterface::UDPInterface(EventProcessorBase *ep, QObject *parent) : QObject(parent){
    eventProcessor = ep;
    mileStone = 0;

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 8991);

    connect(socket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
}

UDPInterface::~UDPInterface(){
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
    if(numBytes%FRAME_LENGTH != 0){
        printf("Incorrect data size: %d bytes\n",numBytes);
        return;
    }
    for(int i = 0; i < numBytes; i+=FRAME_LENGTH){
        Event event;
        unsigned int rawAddr  = ((data[i+1] & 0xff) | ((data[i] & 0xff) << 8));
        if((rawAddr & (0x8000)) != 0){  //special event
            event.special = true;
        }
        else{
            event.polarity = 1 - rawAddr & 1;
            event.posX = (rawAddr >> 1) & 0x7f;
            event.posY = (rawAddr >> 8) & 0x7f;
            event.timeStamp = (data[i+5] & 0xff | ((data[i+4] & 0xff) << 8) |
                               ((data[i+3] & 0xff) << 16) | ((data[i+2] & 0xff) << 24));
        }
//        printf("ts: %d\n",event.timeStamp);
        eventProcessor->getEventBuffer()->add(event);
    }
}

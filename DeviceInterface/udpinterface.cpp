#include "udpinterface.h"

#define DVS128_FRAME_LENGTH 4

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
    if(numBytes%DVS128_FRAME_LENGTH != 0){
        printf("Incorrect data size: %d bytes\n",numBytes);
        return;
    }
    for(int i = 0; i < numBytes; i+=DVS128_FRAME_LENGTH){
        if((data[i+3] & 0x80) == 0x80){
            mileStone += 0x4000L;
        }
        else if ((data[i+3] & 0x40) == 0x40){
            mileStone = 0;
        }
        else{
            Event event;
            unsigned int rawAddr  = (data[i+0] & 0xFF) | ((data[i+1] & 0xFF) << 8);
            if((rawAddr & (0x8000)) != 0){  //special event
                event.special = true;
            }
            else{
                event.polarity = 1 - rawAddr & 1;
                event.posX = (rawAddr >> 1) & 0x7f;
                event.posY = (rawAddr >> 8) & 0x7f;
                event.timeStamp = mileStone + (data[i+2] & 0xff | ((data[i+3] & 0xff) << 8));

            }
            eventProcessor->getEventBuffer()->add(event);
        }
    }
}

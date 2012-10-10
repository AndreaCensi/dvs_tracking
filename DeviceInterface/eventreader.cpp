#include "eventreader.h"
#include "stdio.h"
#include <QUdpSocket>

QUdpSocket sock;

EventReader::EventReader()
{
}

void EventReader::processEvent(Event event){
    char data[4];
    data[0] = (event.yAddr & 0xFF);
    data[1] = ((event.xAddr & 0xFF) << 1) | event.polarity;
    data[2] = (event.timeStamp >> 8) & 0xFF;
    data[3] = event.timeStamp & 0xFF;

    sock.writeDatagram((const char*)&data,4,QHostAddress::LocalHost,8991);
}

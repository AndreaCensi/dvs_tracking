#include "eventprocessor.h"
#include "stdio.h"
#include <QUdpSocket>
#include <math.h>

QUdpSocket sock;

EventProcessor::EventProcessor()
{
}

void EventProcessor::processEvent(Event event){
    //    char data[4];
    //    data[0] = (event.yAddr & 0xFF);
    //    data[1] = ((event.xAddr & 0xFF) << 1) | event.polarity;
    //    data[2] = (event.timeStamp >> 8) & 0xFF;
    //    data[3] = event.timeStamp & 0xFF;

    //sock.writeDatagram((const char*)&data,4,QHostAddress::LocalHost,8991);

    if(clusters.empty()){
        Cluster c;
        c.addEvent(event);
    }
    else{

    }
}

//distance with circular boundary
float EventProcessor::distance(Event e, Cluster c){
    return sqrt(pow(float(e.posX-c.posX),2) + pow(float(e.posY-c.posY),2));
}

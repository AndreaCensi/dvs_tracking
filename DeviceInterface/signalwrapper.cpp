#include "signalwrapper.h"

SignalWrapper::SignalWrapper(QObject *parent) : QObject(parent)
{
    sock = new QUdpSocket;
}

SignalWrapper::~SignalWrapper(){
    delete sock;
}

void SignalWrapper::signalEvent(int x, int y, int type){
    publishEvent(x,y,type);
}

void SignalWrapper::sendDatagram(Event e){
    //sock->writeDatagram();
}

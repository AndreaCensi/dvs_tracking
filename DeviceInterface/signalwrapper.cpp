#include "signalwrapper.h"

SignalWrapper::SignalWrapper(QObject *parent) : QObject(parent)
{
}

void SignalWrapper::sendEvent(int x, int y, int type){
    publishEvent(x,y,type);
}

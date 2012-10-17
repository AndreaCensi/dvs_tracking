#ifndef SIGNALWRAPPER_H
#define SIGNALWRAPPER_H

#include <QObject>
#include "QUdpSocket"
#include "event.h"

class SignalWrapper : public QObject
{
    Q_OBJECT

public:
    SignalWrapper(QObject *parent = 0);
    ~SignalWrapper();
    void signalEvent(int x, int y, int type);
    void sendDatagram(Event e);

private:
    QUdpSocket *sock;

signals:
    void publishEvent(int x, int y, int type);
};

#endif // SIGNALWRAPPER_H

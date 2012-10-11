#ifndef SIGNALWRAPPER_H
#define SIGNALWRAPPER_H

#include <QObject>
#include "event.h"

class SignalWrapper : public QObject
{
    Q_OBJECT

public:
    SignalWrapper(QObject *parent = 0);
    void sendEvent(int x, int y, int type);

signals:
    void publishEvent(int x, int y, int type);
};

#endif // SIGNALWRAPPER_H

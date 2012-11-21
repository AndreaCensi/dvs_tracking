#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include "eventprocessorbase.h"

class UDPInterface : public QObject
{
    Q_OBJECT
public:
    explicit UDPInterface(EventProcessorBase *ep, QObject *parent = 0);
    ~UDPInterface();
    void run();
signals:
    
public slots:
    void readPendingDatagrams();
private:
    void readEvents(QByteArray data);
    QUdpSocket *socket;
    EventProcessorBase *eventProcessor;
    unsigned int mileStone;
};

#endif // UDPINTERFACE_H

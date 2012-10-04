#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = 0);
    void connect(char *host, int port);
    void send(char *data);

private:
    QUdpSocket *socket;
    
signals:
    
public slots:
    
};

#endif // UDPCLIENT_H

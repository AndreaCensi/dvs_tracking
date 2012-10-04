#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QtNetwork/QUdpSocket>

class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = 0);
    void connect(QHostAddress host, int port);
    void send(QByteArray data, QString host, int port);

private:
    QUdpSocket *socket;
    
signals:
    
public slots:
    
};

#endif // UDPCLIENT_H

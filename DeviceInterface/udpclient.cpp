#include "udpclient.h"

UDPClient::UDPClient(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket;
}

void UDPClient::connect(QHostAddress host, int port){
    socket = new QUdpSocket(this);
    socket->bind(host,port);
}

void UDPClient::send(QByteArray data, QString host, int port){
    socket->writeDatagram(data,QHostAddress(host),port);
}

#include "udpclient.h"

UDPClient::UDPClient(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket;
}

void UDPClient::connect(char *host, int port){

}

void UDPClient::send(char *data){

}

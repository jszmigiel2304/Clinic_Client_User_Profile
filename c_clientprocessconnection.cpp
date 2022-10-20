#include "c_clientprocessconnection.h"

c_clientProcessConnection::c_clientProcessConnection(QByteArray serverIdentifier, QObject *parent)
    : QObject{parent}, serverIdentifier(serverIdentifier)
{
    socket = new QLocalSocket();

    //from QLocalSocket
    connect(this->socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this->socket, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)), this, SLOT(errorOccurred(QLocalSocket::LocalSocketError)));
    connect(this->socket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
    //from QIODevice
    connect(this->socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
    connect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64 bytes)));
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //from c_clientProcessConnection
    connect(this, SIGNAL(), this, SLOT());
    void dataRead(quint64 size, QByteArray data, qintptr socketDescriptor);
    void sendDataToClient(myStructures::packet packet);


}

void c_clientProcessConnection::processData(myStructures::threadData data)
{

}

w_logsWindow *c_clientProcessConnection::getLogsWindow() const
{
    return logsWindow;
}

void c_clientProcessConnection::setLogsWindow(w_logsWindow *newLogsWindow)
{
    logsWindow = newLogsWindow;
}

void c_clientProcessConnection::connected()
{

}

void c_clientProcessConnection::disconnected()
{

}

void c_clientProcessConnection::errorOccurred(QLocalSocket::LocalSocketError socketError)
{

}

void c_clientProcessConnection::stateChanged(QLocalSocket::LocalSocketState socketState)
{

}

void c_clientProcessConnection::aboutToClose()
{

}

void c_clientProcessConnection::bytesWritten(qint64 bytes)
{

}

void c_clientProcessConnection::readyRead()
{

}

void c_clientProcessConnection::parseReceivedPacket(quint64 size, QByteArray data, qintptr socketDescriptor)
{
        c_Parser parser;
        QPair<QByteArray, QByteArray> receivedDataFromServer = parser.parseData(size, data);
        myStructures::threadData attchedData;
        parser.parseJson( &receivedDataFromServer.second, &attchedData );

}

void c_clientProcessConnection::passDataToClient(myStructures::packet packet)
{
    socket->write( QString("PACKET_BEGINNING\n").toUtf8() );
    socket->write( packet.packet_to_send );
    socket->write( QString("PACKET_END\n").toUtf8() );

    QString log = QString("%1 has been written. \n").arg(packet.packet_to_send.size());
}

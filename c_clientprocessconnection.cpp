#include "c_clientprocessconnection.h"
#include "c_modulecontroller.h"

#define _PARENT_MOD_CTRLR_ dynamic_cast<c_moduleController *>(parent())

c_clientProcessConnection::c_clientProcessConnection(QByteArray serverIdentifier, QObject *parent)
    : QObject{parent}, serverIdentifier(serverIdentifier)
{
    configuredCorrectly = false;
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
    connect(this, SIGNAL(sendDataToClient(myStructures::packet)), this, SLOT(passDataToClient(myStructures::packet)));

}

c_clientProcessConnection::~c_clientProcessConnection()
{
    socket->disconnectFromServer();
    socket->deleteLater();
}

void c_clientProcessConnection::establishConnection()
{
    socket->connectToServer(serverIdentifier.toHex(), QIODeviceBase::ReadWrite);
}

void c_clientProcessConnection::processData(myStructures::threadData data)
{
    if(data.thread_dest == myTypes::CLINIC_MODULE_CONNECTION_CONTROLLER  || data.thread_dest == myTypes::CLINIC_MODULE_ERROR)
    {
        c_actionExecutive *executive = new c_actionExecutive(this);
        connect(executive, SIGNAL(connectionEstablishedConfirmationReceived()), _PARENT_MOD_CTRLR_, SLOT(moduleConnectedWithLocalServer()));

        executive->processData(data);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku
    }
}

void c_clientProcessConnection::replyReceived(QByteArray processedRequestMd5Hash, QByteArray json)
{
    c_Parser parser;

    myStructures::packet packet;
    packet.md5_hash = parser.getJsonMD5Hash( QJsonDocument::fromJson(json) );
    packet.wait_for_reply = false;

    QDataStream ds2(&packet.packet_to_send, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << packet.md5_hash.toHex() << json;
    emit sendDataToClient(packet);
}

bool c_clientProcessConnection::getConfiguredCorrectly() const
{
    return configuredCorrectly;
}

void c_clientProcessConnection::setConfiguredCorrectly(bool newConfiguredCorrectly)
{
    configuredCorrectly = newConfiguredCorrectly;
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

    QByteArray myPack;

    while(socket->canReadLine()) {
        QByteArray line = QByteArray( socket->readLine() );
        if( QString::fromUtf8(line) == QString("PACKET_BEGINNING\n") ) {
            myPack.clear();
        } else if( QString::fromUtf8(line) == QString("PACKET_END\n") ) {
            emit dataReceived(myPack.size(), myPack);
        } else {
            myPack.append(line);
        }
    }
}


void c_clientProcessConnection::passDataToClient(myStructures::packet packet)
{
    socket->write( QString("PACKET_BEGINNING\n").toUtf8() );
    socket->write( packet.packet_to_send );
    socket->write( QString("PACKET_END\n").toUtf8() );
}

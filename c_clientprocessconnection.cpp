#include "c_clientprocessconnection.h"

c_clientProcessConnection::c_clientProcessConnection(QByteArray serverIdentifier, QObject *parent)
    : QObject{parent}, serverIdentifier(serverIdentifier)
{
    logsWindow = w_logsWindow::Instance();

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

    if(socket->state() == QLocalSocket::ConnectedState)
        logsWindow->addLog( QString("Nawiązano połączenie z QLocalServer: %1").arg(serverIdentifier.toHex()) );
    else
        logsWindow->addLog( QString("Nie Nawiązano połączenia z QLocalServer: %1").arg(serverIdentifier.toHex()) );
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

void c_clientProcessConnection::connected()
{    
    logsWindow->addLog( QString("c_clientProcessConnection::connected() \n") );
}

void c_clientProcessConnection::disconnected()
{
    logsWindow->addLog( QString("c_clientProcessConnection::disconnected() \n") );
}

void c_clientProcessConnection::errorOccurred(QLocalSocket::LocalSocketError socketError)
{
    logsWindow->addLog( QString("c_clientProcessConnection::errorOccurred(QLocalSocket::LocalSocketError socketError) \n") );
    //LocalSocketError { ConnectionRefusedError, PeerClosedError, ServerNotFoundError, SocketAccessError, SocketResourceError, …, UnknownSocketError }
    switch(socketError) {
    case QLocalSocket::ConnectionRefusedError: {logsWindow->addLog( QString("ConnectionRefusedError \n") ); return;}
    case QLocalSocket::PeerClosedError: {logsWindow->addLog( QString("PeerClosedError \n") ); return;}
    case QLocalSocket::ServerNotFoundError: {logsWindow->addLog( QString("ServerNotFoundError \n") ); return;}
    case QLocalSocket::SocketAccessError: {logsWindow->addLog( QString("SocketAccessError \n") ); return;}
    case QLocalSocket::SocketResourceError: {logsWindow->addLog( QString("SocketResourceError \n") ); return;}
    case QLocalSocket::SocketTimeoutError: {logsWindow->addLog( QString("SocketTimeoutError \n") ); return;}
    case QLocalSocket::DatagramTooLargeError: {logsWindow->addLog( QString("DatagramTooLargeError \n") ); return;}
    case QLocalSocket::ConnectionError: {logsWindow->addLog( QString("ConnectionError \n") ); return;}
    case QLocalSocket::UnsupportedSocketOperationError: {logsWindow->addLog( QString("UnsupportedSocketOperationError \n") ); return;}
    case QLocalSocket::OperationError: {logsWindow->addLog( QString("OperationError \n") ); return;}
    case QLocalSocket::UnknownSocketError: {logsWindow->addLog( QString("UnknownSocketError \n") ); return;}
    default: {logsWindow->addLog( QString("UnknownSocketError \n") ); return;}
    }
}

void c_clientProcessConnection::stateChanged(QLocalSocket::LocalSocketState socketState)
{
    logsWindow->addLog( QString("c_clientProcessConnection::stateChanged(QLocalSocket::LocalSocketState socketState) \n") );
    switch (socketState) {
    case QLocalSocket::UnconnectedState: {logsWindow->addLog( QString("UnconnectedState \n") ); return;}
    case QLocalSocket::ConnectingState: {logsWindow->addLog( QString("ConnectingState \n") ); return;}
    case QLocalSocket::ConnectedState: {logsWindow->addLog( QString("ConnectedState \n") ); return;}
    case QLocalSocket::ClosingState: {logsWindow->addLog( QString("ClosingState \n") ); return;}
    default: {logsWindow->addLog( QString("No state att. \n") ); return;}
    }
}

void c_clientProcessConnection::aboutToClose()
{
    logsWindow->addLog( QString("c_clientProcessConnection::aboutToClose() \n") );
}

void c_clientProcessConnection::bytesWritten(qint64 bytes)
{
    logsWindow->addLog( QString("c_clientProcessConnection::bytesWritten(qint64 bytes) Zapisano [ %1 ] bytes\n").arg(bytes) );
}

void c_clientProcessConnection::readyRead()
{        
    logsWindow->addLog( QString("c_clientProcessConnection::readyRead() - zaczynam czytać \n") );

    QByteArray myPack;

    while(socket->canReadLine()) {
        QByteArray line = QByteArray( socket->readLine() );
        if( QString::fromUtf8(line) == QString("PACKET_BEGINNING\n") ) {
            myPack.clear();
        } else if( QString::fromUtf8(line) == QString("PACKET_END\n") ) {
            QString log = QString("%1 has been read. \n").arg(myPack.size());
            emit newLog(log);

            emit dataReceived(myPack.size(), myPack);
        } else {
            myPack.append(line);
        }
    }
}


void c_clientProcessConnection::passDataToClient(myStructures::packet packet)
{
    logsWindow->addLog( QString("c_clientProcessConnection::passDataToClient(myStructures::packet packet) - wysyłam pakiet \n") );
    socket->write( QString("PACKET_BEGINNING\n").toUtf8() );
    socket->write( packet.packet_to_send );
    socket->write( QString("PACKET_END\n").toUtf8() );

    QString log = QString("%1 has been written. \n").arg(packet.packet_to_send.size());
}

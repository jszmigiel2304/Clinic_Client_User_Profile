#include "c_modulecontroller.h"

c_moduleController::c_moduleController(QByteArray serverId, QByteArray moduleId, qint32 threadId, QObject *parent)
    : QObject(parent), serverIdentifier(serverId), moduleIdentifier(moduleId), threadIdentifier(threadId)
{
    logsWindow = w_logsWindow::Instance();
    connection = new c_clientProcessConnection(  this->getServerIdentifier() );
    connect(this, SIGNAL(aboutToClose()), connection, SLOT(deleteLater()));
    connect(connection, SIGNAL(dataReceived(quint64, QByteArray)), this, SLOT(dataReceived(quint64, QByteArray)) );

}

c_moduleController::~c_moduleController()
{

}

w_logsWindow *c_moduleController::getLogsWindow() const
{
    return logsWindow;
}

void c_moduleController::setLogsWindow(w_logsWindow *newLogsWindow)
{
    logsWindow = newLogsWindow;
}

void c_moduleController::processData(myStructures::threadData data)
{
    logsWindow->addLog(data.toString());

    if(data.thread_dest == myTypes::CLINIC_MODULE  || data.thread_dest == myTypes::CLINIC_MODULE_ERROR)
    {
        c_actionExecutive *executive = new c_actionExecutive(this);
        connect(executive, SIGNAL(replyReady(QByteArray, QByteArray)), getConnection(), SLOT(replyReceived(QByteArray, QByteArray)));


        QString errMsg = QString("TPrzetwarzam otrzymana paczke. \n");
        logsWindow->addLog(errMsg);

        executive->processData(data);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku

        QString errMsg = QString("Thread ERROR. \n Wrong THREAD DESTINATION or THREAD ID");
        logsWindow->addLog(errMsg);
    }
}

myTypes::ThreadDestination c_moduleController::getNameThreadDestination() const
{
    return nameThreadDestination;
}

void c_moduleController::dataReceived(quint64 data_size, QByteArray data)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> receivedDataFromServer = parser.parseData(data_size, data);
    myStructures::threadData attchedData;
    parser.parseJson( &receivedDataFromServer.second, &attchedData );

    processData(attchedData);

//    if(attchedData.content == myTypes::PACKET_RECEIVE_CONFIRMATION)
//        emit packetReceiveConfirmationReceived(attchedData);
//    else {
//        emit replyReceived( attchedData.ref_md5 );
//        emit passDataToThread(attchedData);
//    }
}

void c_moduleController::dataReceived(myStructures::threadData data)
{
    this->processData(data);
}

c_clientProcessConnection *c_moduleController::getConnection() const
{
    return connection;
}

void c_moduleController::setConnection(c_clientProcessConnection *newConnection)
{
    connection = newConnection;
}

void c_moduleController::connectToLocalServer()
{
    getConnection()->establishConnection();
}

w_MainWindow *c_moduleController::getMainWnd() const
{
    return mainWnd;
}

void c_moduleController::setMainWnd(w_MainWindow *newMainWnd)
{
    mainWnd = newMainWnd;
}

qint32 c_moduleController::getThreadIdentifier() const
{
    return threadIdentifier;
}

void c_moduleController::setThreadIdentifier(qint32 newThreadIdentifier)
{
    threadIdentifier = newThreadIdentifier;
}

const QByteArray &c_moduleController::getModuleIdentifier() const
{
    return moduleIdentifier;
}

void c_moduleController::setModuleIdentifier(const QByteArray &newModuleIdentifier)
{
    moduleIdentifier = newModuleIdentifier;
}

const QByteArray &c_moduleController::getServerIdentifier() const
{
    return serverIdentifier;
}

void c_moduleController::setServerIdentifier(const QByteArray &newServerIdentifier)
{
    serverIdentifier = newServerIdentifier;
}

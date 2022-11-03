#include "c_modulecontroller.h"

c_moduleController::c_moduleController(QByteArray serverId, QByteArray moduleId, qint32 threadId, QObject *parent)
    : QObject(parent), serverIdentifier(serverId), moduleIdentifier(moduleId), threadIdentifier(threadId)
{
    connection = new c_clientProcessConnection(  this->getServerIdentifier(), this );
    mainWnd = new w_MainWindow();
    userCtrlr = nullptr;
    waitingLoop = new c_waitingLoop::c_waitingLoop(this);


    connect(this, SIGNAL(aboutToClose()), connection, SLOT(deleteLater()));
    connect(connection, SIGNAL(dataReceived(quint64, QByteArray)), this, SLOT(dataReceived(quint64, QByteArray)) );

}

c_moduleController::~c_moduleController()
{
    connection->deleteLater();
    mainWnd->deleteLater();
    userCtrlr->deleteLater();
    waitingLoop->deleteLater();
}

void c_moduleController::processData(myStructures::threadData data)
{
    if(data.thread_dest == myTypes::CLINIC_MODULE  || data.thread_dest == myTypes::CLINIC_MODULE_ERROR)
    {
        c_actionExecutive *executive = new c_actionExecutive(this);
        connect(executive, SIGNAL(replyToConnectionRequestReady(QByteArray, QByteArray)), getConnection(), SLOT(replyReceived(QByteArray, QByteArray)));
        connect( executive, SIGNAL(userLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>>)), this, SLOT(userLogsReceivedFromServer(QList<QMap<QString, QVariant>>)) );
        connect( executive, SIGNAL(userEmployeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>>)), this, SLOT(userEmployeeLogsReceivedFromServer(QList<QMap<QString, QVariant>>)) );
        connect( executive, SIGNAL(employeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>>)), this, SLOT(emlpoyeeLogsReceivedFromServer(QList<QMap<QString, QVariant>>)) );
        connect( executive, SIGNAL(employeePropertiesReceivedFromServerResultReady(QMap<QString, QVariant>)), this, SLOT(employeePropertiesReceivedFromServer(QMap<QString, QVariant>)) );
        connect( executive, SIGNAL(userPropertiesReceivedFromServerResultReady(QMap<QString, QVariant>)), this, SLOT(userPropertiesReceivedFromServer(QMap<QString, QVariant>)) );

        executive->processData(data);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku
    }
}

void c_moduleController::updateMainWindow()
{
    if(userCtrlr->getDbLogs().isEmpty() || userCtrlr->getId() == 0 || userCtrlr->getEmployee()->getId() == 0) {
        connect( this, SIGNAL(getWindowProperties(QMap<QString, QVariant> *, QMap<QString, QVariant> *, QList<myStructures::myLog> * )),
                 this, SLOT(getPropertiesFromServer(QMap<QString, QVariant> *, QMap<QString, QVariant> *, QList<myStructures::myLog> *)), Qt::DirectConnection );

        waitingLoop->setExpireTime(15000);

        connect(waitingLoop, SIGNAL(loopStarted(QString)), mainWnd, SLOT(processing(QString)));
        connect(waitingLoop, SIGNAL(exitLoop(int)), mainWnd, SLOT(processingFinished(int)));

        if(userCtrlr->getId() == 0) {
            connect(userCtrlr, SIGNAL(propertiesSaved()), waitingLoop->newCondition(), SLOT(conditionFulfilled()) , Qt::DirectConnection);
            connect(userCtrlr, SIGNAL(passProperties(QMap<QString, QVariant>)), mainWnd, SLOT(setUserProperties(QMap<QString, QVariant>)), Qt::DirectConnection );
        }
        if(userCtrlr->getEmployee()->getId() == 0) {
            connect(userCtrlr->getEmployee(), SIGNAL(propertiesSaved()), waitingLoop->newCondition(), SLOT(conditionFulfilled()), Qt::DirectConnection );
            connect(userCtrlr->getEmployee(), SIGNAL(passProperties(QMap<QString, QVariant>)), mainWnd, SLOT(setEmployeeProperties(QMap<QString, QVariant>)), Qt::DirectConnection );
        }
        if(userCtrlr->getDbLogs().isEmpty()) {
            connect(userCtrlr, SIGNAL(logsSaved()), waitingLoop->newCondition(), SLOT(conditionFulfilled()), Qt::DirectConnection );
            connect(userCtrlr, SIGNAL(passLogs(QList<myStructures::myLog>)), mainWnd, SLOT(setLogs(QList<myStructures::myLog>)), Qt::DirectConnection );
        }
    }

    emit getWindowProperties(mainWnd->getUserProperties(), mainWnd->getEmployeeProperties(), mainWnd->getLogs());

    if(waitingLoop->getConditionsNumber() > 0) {
        waitingLoop->startExec();
    }

    mainWnd->refresh();
}

void c_moduleController::moduleConnectedWithLocalServer()
{
    connection->setConfiguredCorrectly(true);
    emit moduleConnectedWithServer();
}

c_userController *c_moduleController::getUserCtrlr() const
{
    return userCtrlr;
}

void c_moduleController::setUserCtrlr(c_userController *newUserCtrlr)
{
    userCtrlr = newUserCtrlr;
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

    switch (attchedData.thread_dest) {
    case myTypes::CLINIC_MODULE: {processData(attchedData); break;};
    case myTypes::CLINIC_MODULE_ERROR: {processData(attchedData); break;};
    case myTypes::CLINIC_MODULE_CONNECTION_CONTROLLER: {
        connection->processData(attchedData);
        break;};
    default: { processData(attchedData); break;};
    }
}

void c_moduleController::getUserPropertiesFromServer(qint32 id, QString name, QString password)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetUserPropertiesPacket(name, password, getNameThreadDestination(), getThreadIdentifier());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit connection->sendDataToClient(packet);
}

void c_moduleController::getEmployeePropertiesFromServer(qint32 id, QString name, QString password)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetEmployeePropertiesPacket(name, password, getNameThreadDestination(), getThreadIdentifier());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit connection->sendDataToClient(packet);
}

void c_moduleController::getLogsFromServer(qint32 id, QString name, QString password)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetLogsPacket(id, name, password, getNameThreadDestination(), getThreadIdentifier());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit connection->sendDataToClient(packet);
}

void c_moduleController::userPropertiesReceivedFromServer(QMap<QString, QVariant> properties)
{
    getUserCtrlr()->setProperties(properties);
}

void c_moduleController::employeePropertiesReceivedFromServer(QMap<QString, QVariant> properties)
{
    getUserCtrlr()->getEmployee()->setProperties(properties);
}

void c_moduleController::userLogsReceivedFromServer(QList<QMap<QString, QVariant> > logs)
{
    getUserCtrlr()->setDbLogs(logs);
}

void c_moduleController::userEmployeeLogsReceivedFromServer(QList<QMap<QString, QVariant> > logs)
{
    getUserCtrlr()->setDbLogs(logs);
}

void c_moduleController::emlpoyeeLogsReceivedFromServer(QList<QMap<QString, QVariant> > logs)
{
    getUserCtrlr()->setDbLogs(logs);
}

void c_moduleController::connected()
{
    updateMainWindow();
}

void c_moduleController::getPropertiesFromServer(QMap<QString, QVariant> *userProperties, QMap<QString, QVariant> *employeeProperties, QList<myStructures::myLog> *Logs)
{
    if(userCtrlr->getId() == 0 || userCtrlr->getName().isEmpty()) {
        getUserPropertiesFromServer();
    } else {
        (*userProperties) = userCtrlr->getUserProperties();
    }

    if(userCtrlr->getEmployee()->getId() == 0 || userCtrlr->getEmployee()->getName().isEmpty()) {
        getEmployeePropertiesFromServer();
    } else {
        (*employeeProperties) = userCtrlr->getEmployee()->getProperties();
    }

    if(userCtrlr->getDbLogs().isEmpty()) {
        this->getLogsFromServer();
    } else {
        (*Logs) = userCtrlr->getDbLogs();
    }
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
    waitingLoop->setExpireTime(15000);

    connect(waitingLoop, SIGNAL(loopStarted(QString)), mainWnd, SLOT(processing(QString)));
    connect(waitingLoop, SIGNAL(exitLoop(int)), mainWnd, SLOT(processingFinished(int)));

    connect(this, SIGNAL(moduleConnectedWithServer()), waitingLoop->newCondition(), SLOT(conditionFulfilled()) );
    connect(this, SIGNAL(moduleConnectedWithServer()), this, SLOT(connected()) );

    getConnection()->establishConnection();

    if(waitingLoop->getConditionsNumber() > 0) {
        waitingLoop->startExec();
    }
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

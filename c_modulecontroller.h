#ifndef C_MODULECONTROLLER_H
#define C_MODULECONTROLLER_H
#include "cv_processdata.h"
#include "c_myStructures.h"
#include "w_logswindow.h"
#include "w_mainwindow.h"
#include "c_clientprocessconnection.h"
#include "c_actionexecutive.h"
#include "c_waitingloop.h"
#include "c_usercontroller.h"

#include <QObject>
#include <QObject>
#include <QMessageBox>

class c_moduleController : public QObject, public cv_ProcessData
{
    Q_OBJECT
public:
    explicit c_moduleController(QByteArray serverId, QByteArray moduleId, qint32 threadId, QObject *parent = nullptr);
    ~c_moduleController();

    w_logsWindow *getLogsWindow() const;
    void setLogsWindow(w_logsWindow *newLogsWindow);


    const QByteArray &getServerIdentifier() const;
    void setServerIdentifier(const QByteArray &newServerIdentifier);

    const QByteArray &getModuleIdentifier() const;
    void setModuleIdentifier(const QByteArray &newModuleIdentifier);

    qint32 getThreadIdentifier() const;
    void setThreadIdentifier(qint32 newThreadIdentifier);

    w_MainWindow *getMainWnd() const;
    void setMainWnd(w_MainWindow *newMainWnd);

    c_clientProcessConnection *getConnection() const;
    void setConnection(c_clientProcessConnection *newConnection);

    void connectToLocalServer();

    myTypes::ThreadDestination getNameThreadDestination() const;

    c_userController *getUserCtrlr() const;
    void setUserCtrlr(c_userController *newUserCtrlr);

public slots:
    void dataReceived(myStructures::threadData data);
    void processData(myStructures::threadData data) override;
    void updateMainWindow();
    void moduleConnectedWithLocalServer();


private:
    const myTypes::ThreadDestination nameThreadDestination = myTypes::CLINIC_MODULE;
    w_logsWindow * logsWindow;

    QByteArray serverIdentifier;//służy w połączeniu do odpiweiniego QLocalServer, c_processesControllerThread::QLocalServer *localServer; w Clinic Client
    QByteArray moduleIdentifier;//służy w połączeniu z c_processesController::QList<c_moduleProcess *> openedModulesProcesses w Clinic Client
    qint32 threadIdentifier;    //służy w pakietach wysłanych do serwera, nie ma niz wspólnego z QThread::currentThreadId() !!!!!!!!!!!!!!!!!!!!!!!!!!!

    w_MainWindow * mainWnd;
    c_clientProcessConnection *connection;

    c_waitingLoop::c_waitingLoop *waitingLoop;

    c_userController *userCtrlr;

private slots:
    void dataReceived(quint64 data_size, QByteArray data);
    void getPropertiesFromServer(QMap<QString, QVariant> *userProperties, QMap<QString, QVariant> *employeeProperties, QList<myStructures::myLog> * Logs);
    void getUserPropertiesFromServer(qint32 id = -1, QString name = QString(""), QString password = QString(""));
    void getEmployeePropertiesFromServer(qint32 id = -1, QString name = QString(""), QString password = QString(""));
    void getLogsFromServer(qint32 id = -1, QString name = QString(""), QString password = QString(""));

signals:
    void aboutToClose();
    void getWindowProperties(QMap<QString, QVariant> * userProperties, QMap<QString, QVariant> * employeeProperties, QList<myStructures::myLog> * Logs);
    void moduleConnectedWithServer();

};

#endif // C_MODULECONTROLLER_H

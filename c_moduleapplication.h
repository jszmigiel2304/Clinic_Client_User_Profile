#ifndef C_MODULEAPPLICATION_H
#define C_MODULEAPPLICATION_H

#include <QApplication>
#include <QObject>

class c_moduleApplication : public QApplication
{
    Q_OBJECT
public:
    c_moduleApplication(int &argc, char **argv);

    const QByteArray &getProcessIdentifier() const;
    void setProcessIdentifier(const QByteArray &newProcessIdentifier);

    qint32 getThreadIdentifier() const;
    void setThreadIdentifier(qint32 newThreadIdentifier);

private:
    QByteArray processIdentifier;   //służy w połączeniu z c_processesController::QList<c_moduleProcess *> openedModulesProcesses w Clinic Client
    qint32 threadIdentifier;    //służy w pakietach wysłanych do serwera, nie ma niz wspólnego z QThread::currentThreadId() !!!!!!!!!!!!!!!!!!!!!!!!!!!

};

#endif // C_MODULEAPPLICATION_H

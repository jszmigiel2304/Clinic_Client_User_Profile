#ifndef C_CLIENTPROCESSCONNECTION_H
#define C_CLIENTPROCESSCONNECTION_H

#include "c_myStructures.h"
#include "cv_processdata.h"
#include "w_logswindow.h"
#include "c_parser.h"
#include "c_moduleapplication.h"

#include <QObject>
#include <QLocalSocket>
#include <QAbstractSocket>
#include <QDataStream>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QList>
#include <QTime>
#include <QTimer>
#include <QApplication>

class c_clientProcessConnection : public QObject, public cv_ProcessData
{
    Q_OBJECT
public:
    enum LocalJsonContent { EMPTY = 0x00000000, ERRORS = 0x00000001, CONNECTION_TO_PROCESS_REQUEST = 0x00000002};

    explicit c_clientProcessConnection(QByteArray serverIdentifier, QObject *parent = nullptr);
    ~c_clientProcessConnection();

    void establishConnection();

    void processData(myStructures::threadData data) override;

    w_logsWindow *getLogsWindow() const;
    void setLogsWindow(w_logsWindow *newLogsWindow);

public slots:
    void replyReceived(QByteArray processedRequestMd5Hash, QByteArray json);

private:
    QLocalSocket *socket;
    QByteArray serverIdentifier; //służy w połączeniu do odpiweiniego QLocalServer, c_processesControllerThread::QLocalServer *localServer; w Clinic Client
    bool configuredCorrectly;   // true - gdy Clinic Client poprawnie połączy QLocalSocket(c_moduleProcessConnection) z c_moduleProcess( QProcess )
    const myTypes::ThreadDestination nameThreadDestination = myTypes::CLINIC_MODULE_CONNECTION_CONTROLLER;

    w_logsWindow * logsWindow;

private slots:
    //from QLocalSocket
    void connected();
    void disconnected();
    void errorOccurred(QLocalSocket::LocalSocketError socketError);
    void stateChanged(QLocalSocket::LocalSocketState socketState);

    //from QIODevice
    void aboutToClose();
    void bytesWritten(qint64 bytes);
    void readyRead();

    void passDataToClient(myStructures::packet packet);

signals:
    void newLog(QString log);
    void error(QLocalSocket::LocalSocketError error);
    void sendDataToClient(myStructures::packet packet);
    void dataReceived(quint64 data_size, QByteArray data);
};

#endif // C_CLIENTPROCESSCONNECTION_H

#ifndef C_ACTIONEXECUTIVE_H
#define C_ACTIONEXECUTIVE_H

#include "c_myStructures.h"
#include "c_parser.h"

#include <QObject>
#include <QAuthenticator>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

class c_actionExecutive : public QObject
{
    Q_OBJECT
public:
    explicit c_actionExecutive(QObject *parent = nullptr);
    ~c_actionExecutive();

    void processData(myStructures::threadData data);

public slots:

protected:

private:
   QMap<QString, QString> processedDataErrors;

    void processMessage(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processGet(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processReply(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processRequest(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);


private slots:
    void processResults(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant>> results);
    void processErrors(QMap<QString, QString> * receivedErrors);

signals:
    void newLog(QString log);
    void newLogToFile(QString sender, QString notes, QByteArray data);
    void errors(QMap<QString, myStructures::threadData> errors);
    void resultReady(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant>> results);    
    void errors(QMap<QString, QString> * receivedErrors);
    void replyToConnectionRequestReady(QByteArray processedRequestMd5Hash, QByteArray json);

    void connectionEstablishedConfirmationReceived();

    void userLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>> logs);
    void userEmployeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>> logs);
    void employeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>> logs);
    void employeePropertiesReceivedFromServerResultReady(QMap<QString, QVariant> properties);
    void userPropertiesReceivedFromServerResultReady(QMap<QString, QVariant> properties);

};

#endif // C_ACTIONEXECUTIVE_H

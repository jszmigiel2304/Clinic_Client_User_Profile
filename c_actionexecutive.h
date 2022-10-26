#ifndef C_ACTIONEXECUTIVE_H
#define C_ACTIONEXECUTIVE_H

#include "w_logswindow.h"
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


    w_logsWindow *getLogs() const;
    void setLogs(w_logsWindow *value);

    void processData(myStructures::threadData data);


public slots:

protected:

private:
   QMap<QString, QString> processedDataErrors;

    w_logsWindow *logs;

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
    void replyReady(QByteArray processedRequestMd5Hash, QByteArray json);

};

#endif // C_ACTIONEXECUTIVE_H

#include "c_actionexecutive.h"
#include "c_modulecontroller.h"

#define _PARENT_MOD_CTRLR_ dynamic_cast<c_moduleController *>(parent())

c_actionExecutive::c_actionExecutive(QObject *parent)
    : QObject{parent}
{
    connect( this, SIGNAL(resultReady(myTypes::JsonContent, QList<QMap<QString, QVariant>>)), this, SLOT(processResults(myTypes::JsonContent, QList<QMap<QString, QVariant>>)), Qt::DirectConnection );
    connect( this, SIGNAL(errors(QMap<QString, QString> *)), this, SLOT(processErrors(QMap<QString, QString> *)), Qt::DirectConnection );
}

c_actionExecutive::~c_actionExecutive()
{
}

void c_actionExecutive::processData(myStructures::threadData data)
{
        // obsługa otrzymanych danych

        switch (data.req_type) {
        case myTypes::PING:
        {
            break;
        }
        case myTypes::REPLY:
        {
            processReply(data, &processedDataErrors);            
            break;
        }
        case myTypes::MESSAGE:
        {
            processMessage(data, &processedDataErrors);
            break;
        }
        case myTypes::GET:
        {
            processGet(data, &processedDataErrors);
            break;
        }
        case myTypes::REQUEST:
        {
            processRequest(data, &processedDataErrors);
            break;
        }
        case myTypes::UPDATE:
        {
        }
        case myTypes::DELETE:
        {
            break;
        }
        case myTypes::SEND:
        {
            break;
        }
        default:
        {
            break;
        }
        }
}

void c_actionExecutive::processMessage(myStructures::threadData data, QMap<QString, QString> * processedDataErrors)
{

}

void c_actionExecutive::processGet(myStructures::threadData data, QMap<QString, QString> * processedDataErrors)
{

}

void c_actionExecutive::processReply(myStructures::threadData data, QMap<QString, QString> * processedDataErrors)
{
        c_Parser parser;
        myTypes::JsonContent jsonContent = data.content;
        QList<QMap<QString, QVariant>> jsonData = data.data;

        if (data.thread_dest == myTypes::CLINIC_MODULE_ERROR)
            parser.parseErrors(&jsonData, processedDataErrors);

        if(jsonContent == myTypes::EMPTY) { (*processedDataErrors)[QString("JSON_DOC_ERROR")] = QString("Pusty dokument JSON.\n"); emit errors(processedDataErrors);  return; }
        else { emit resultReady(jsonContent, jsonData); }
}

void c_actionExecutive::processRequest(myStructures::threadData data, QMap<QString, QString> *processedDataErrors)
{

    switch (data.type_flag) {
    case 0xFFFF0000:  // Connection to process settings request, sended after module connection established
    {
        c_Parser parser;

        QList<QMap<QString, QVariant>> dataForJSON;
        QMap<QString, QVariant> map;

        map["server_id"] = _PARENT_MOD_CTRLR_->getServerIdentifier().toHex();
        map["module_id"] = _PARENT_MOD_CTRLR_->getModuleIdentifier().toHex();
        map["thread_id"] = _PARENT_MOD_CTRLR_->getThreadIdentifier();

        dataForJSON.append(map);

        QMap<QString, QVariant> packetInfo;
        packetInfo["thread_dest"] = static_cast<qint8>(myTypes::CLINIC_MODULE_PROCESS_CONTROLLER);
        packetInfo["thread_id"] = data.thread_id;
        packetInfo["req_type"] = static_cast<qint8>(myTypes::REPLY);
        packetInfo["type_flag"] = 0x00000001;
        packetInfo["content"] = static_cast<qint32>(myTypes::CONNECTION_TO_PROCESS_ANSWER);
        packetInfo["ref_md5"] = QJsonValue::fromVariant( data.ref_md5.toHex() );


        QJsonDocument replyJSON = parser.prepareJson(packetInfo, dataForJSON);
        emit replyToConnectionRequestReady(data.ref_md5, replyJSON.toJson());

        break;
    }
    }
}

void c_actionExecutive::processResults(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant> > results)
{
    switch (jsonContent) {    
    case myTypes::CONNECTION_ESTABLISHED_CONFIRMATION: {
        emit connectionEstablishedConfirmationReceived();
        break;
    }
    case myTypes::ERRORS: {
        emit errors(&processedDataErrors);
        break;
    }
    case myTypes::USER_PROPERTIES_ANSWER: {
        if(results.isEmpty())
            emit userPropertiesReceivedFromServerResultReady( QMap<QString, QVariant>() );
        else
            emit userPropertiesReceivedFromServerResultReady( results[0] );
        break;
    }
    case myTypes::EMPLOYEE_PROPERTIES_ANSWER: {
        if(results.isEmpty())
            emit employeePropertiesReceivedFromServerResultReady( QMap<QString, QVariant>() );
        else
            emit employeePropertiesReceivedFromServerResultReady( results[0] );
        break;
    }
    case myTypes::USER_LOGS_ANSWER: {
        if(results.isEmpty())
            emit userLogsReceivedFromServerResultReady( QList<QMap<QString, QVariant>>() );
        else
            emit userLogsReceivedFromServerResultReady(results);
        break;
    }
    case myTypes::USER_EMPLOYEE_LOGS_ANSWER: {
        if(results.isEmpty())
            emit userEmployeeLogsReceivedFromServerResultReady( QList<QMap<QString, QVariant>>() );
        else
            emit userEmployeeLogsReceivedFromServerResultReady(results);
        break;
    }
    case myTypes::EMPLOYEE_LOGS_ANSWER: {
        if(results.isEmpty())
            emit employeeLogsReceivedFromServerResultReady( QList<QMap<QString, QVariant>>() );
        else
            emit employeeLogsReceivedFromServerResultReady(results);
        break;
    }
    default:
    {
        break;
    }
    }
}

void c_actionExecutive::processErrors(QMap<QString, QString> *receivedErrors)
{
//    foreach(const QString key, receivedErrors->keys()) {
//        emit newLog(QString("%1: %2 \n").arg(key, (*receivedErrors)[key]));
//    }
    receivedErrors->clear();
}

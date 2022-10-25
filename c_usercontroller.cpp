#include "c_usercontroller.h"

c_userController::c_userController(QObject *parent)
    : m_user{parent}
{
    logs = w_logsWindow::Instance();
}

c_userController::c_userController(QMap<QString, QVariant> properties, QObject *parent)
    : m_user{parent}
{
    logs = w_logsWindow::Instance();
    setProperties(properties);
}

QMap<QString, QVariant> c_userController::getUserProperties()
{
    QMap<QString, QVariant> map;


    map["id"] = this->getId();
    map["name"] = this->getName();
    map["role"] = this->getRoleString();
    map["create_date"] = this->getCreate_date().toString();
    map["email"] = this->getEmail();
    map["is_blocked"] = this->getBlocked();
    map["is_verified"] = this->getVerified();
    map["blocked_date"] = this->getBlock_date().toString();
    map["verified_date"] = this->getVerify_date().toString();
    map["is_logged"] = this->getIsLogged();
    map["photo"] = this->getPhoto();


    return map;
}

QMap<QString, QVariant> c_userController::getEmployeeProperties()
{
    return this->getEmployee()->getProperties(true, true);
}



void c_userController::setProperties(QMap<QString, QVariant> userInfo)
{
    setId( userInfo["id"].toUInt() );
    setName( userInfo["name"].toString() );
    setEmail( userInfo["email"].toString() );
    setVerified( userInfo["verified"].toBool() );
    setBlocked( userInfo["blocked"].toBool() );
    setCreate_date( QDateTime::fromString( userInfo["create_date"].toString() ) );
    setVerify_date( QDateTime::fromString( userInfo["verify_date"].toString() ) );
    setBlock_date( QDateTime::fromString( userInfo["blocked_date"].toString() ) );
    setPhoto( QByteArray( userInfo["photo"].toByteArray() ) );
    QMetaEnum metaEnum = QMetaEnum::fromType<m_user::UserRole>();
    setRole( static_cast<m_user::UserRole>( metaEnum.keyToValue(userInfo["role"].toString().toStdString().c_str() ) ) );
    setIsLogged( userInfo["logged"].toBool() );

    emit propertiesSaved();
    emit passProperties(userInfo);
}

const QList<myStructures::myLog> &c_userController::getDbLogs() const
{
    return dbLogs;
}

void c_userController::setDbLogs(const QList<myStructures::myLog> &newDbLogs)
{
    dbLogs = newDbLogs;
}

void c_userController::setDbLogs(QList<QMap<QString, QVariant> > logs)
{
    for(int i = 0; i< logs.size(); i++) {
        myStructures::myLog log = myStructures::myLog::fromMap(logs[i]);
        this->dbLogs.append(log);
    }

    emit logsSaved();
    emit passLogs(QList<myStructures::myLog>(this->dbLogs));
}

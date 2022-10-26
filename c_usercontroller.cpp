#include "c_usercontroller.h"

c_userController::c_userController(QObject *parent)
    : m_user{parent}
{
    logs = w_logsWindow::Instance();
    setEmployee(new c_employee(true));
    logs->addLog(QString("Utworzono c_userController\n"));
}

c_userController::c_userController(QMap<QString, QVariant> properties, QObject *parent)
    : m_user{parent}
{
    logs = w_logsWindow::Instance();
    setEmployee(new c_employee(true));
    logs->addLog(QString("Utworzono c_userController\n"));
    setProperties(properties);
}

QMap<QString, QVariant> c_userController::getUserProperties()
{
    QMessageBox msgBox;
    msgBox.setText("c_userController::getUserProperties()");
    msgBox.exec();

    if(getId() <= 0 || getName().isEmpty()) return QMap<QString, QVariant>();

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

    logs->addLog(QString("getUserProperties() zwracam mape\n"));
    QMessageBox msgBox2;
    msgBox2.setText("c_userController::getUserProperties() konice");
    msgBox2.exec();

    return map;
}

QMap<QString, QVariant> c_userController::getEmployeeProperties()
{    
    logs->addLog(QString("getEmployeeProperties() zwracam mape\n"));
    QMessageBox msgBox;
    msgBox.setText("c_userController::getEmployeeProperties()");
    msgBox.exec();
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

    logs->addLog(QString("setProperties() ustawiono dane\n"));

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

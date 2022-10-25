#include "m_user.h"

m_user::m_user(QObject *parent) : QObject(parent)
{

}

qint32 m_user::getId() const
{
    return id;
}

void m_user::setId(const qint32 &value)
{
    id = value;
}

QString m_user::getName() const
{
    return name;
}

void m_user::setName(const QString &value)
{
    name = value;
}

QString m_user::getPassword() const
{
    return password;
}

void m_user::setPassword(const QString &value)
{
    password = value;
}

QString m_user::getEncryptedPassword(bool toHex)
{
    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << getPassword();

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    QString hax = encryptedPassword.toHex();

    return (toHex ? encryptedPassword.toHex() : encryptedPassword);
}

QString m_user::getEncryptedPassword(bool hashName, bool toHex)
{
    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << getPassword();
    if(hashName)
        encryptionStream << getName();

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    return (toHex ? encryptedPassword.toHex() : encryptedPassword);
}

QString m_user::getSessionId() const
{
    return sessionId;
}

void m_user::setSessionId(const QString &value)
{
    sessionId = value;
}

QDateTime m_user::getLoginTimeStamp() const
{
    return logInTime;
}

void m_user::setLoginTimeStamp(const QDateTime &value)
{
    logInTime = value;
}

bool m_user::getIsLogged() const
{
    return isLogged;
}

void m_user::setIsLogged(bool value)
{
    isLogged = value;
}

const QString &m_user::getEmail() const
{
    return email;
}

void m_user::setEmail(const QString &newEmail)
{
    email = newEmail;
}

bool m_user::getVerified() const
{
    return verified;
}

void m_user::setVerified(bool newVerified)
{
    verified = newVerified;
}

bool m_user::getBlocked() const
{
    return blocked;
}

void m_user::setBlocked(bool newBlocked)
{
    blocked = newBlocked;
}

m_user::UserRole m_user::getRole() const
{
    return role;
}

QString m_user::getRoleString() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<m_user::UserRole>();
    return metaEnum.valueToKey( this->getRole() );
}

void m_user::setRole(UserRole newRole)
{
    role = newRole;
}

void m_user::clearProperties()
{
    setId(-1);
    name.clear();
    password.clear();
    sessionId.clear();
    logInTime = QDateTime();
    setIsLogged(false);
    email.clear();
    setVerified(false);
    setBlocked(false);
    role = NO_ROLE;

    if(employee->getSupervisor() != nullptr)
        employee->getSupervisor()->deleteLater();

    if(employee != nullptr)
        employee->deleteLater();

    employee = new c_employee(true);
}

myStructures::authenticator m_user::getAuthorizationData()
{
    myStructures::authenticator data;
    data.id = getId();
    data.name = getName();
    data.password = getPassword();
    return data;
}

c_employee *m_user::getEmployee() const
{
    return employee;
}

void m_user::setEmployee(c_employee *newEmployee)
{
    employee = newEmployee;
}

const QDateTime &m_user::getCreate_date() const
{
    return create_date;
}

void m_user::setCreate_date(const QDateTime &newCreate_date)
{
    create_date = newCreate_date;
}

const QDateTime &m_user::getVerify_date() const
{
    return verify_date;
}

void m_user::setVerify_date(const QDateTime &newVerify_date)
{
    verify_date = newVerify_date;
}

const QDateTime &m_user::getBlock_date() const
{
    return block_date;
}

void m_user::setBlock_date(const QDateTime &newBlock_date)
{
    block_date = newBlock_date;
}

const QByteArray &m_user::getPhoto() const
{
    return photo;
}

void m_user::setPhoto(const QByteArray &newPhoto)
{
    photo = newPhoto;
}

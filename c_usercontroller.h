#ifndef C_USERCONTROLLER_H
#define C_USERCONTROLLER_H

#include "m_user.h"
#include "w_logswindow.h"
#include "c_employee.h"
#include "c_parser.h"

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QMetaEnum>
#include <QMessageBox>

class c_userController : public m_user
{
    Q_OBJECT
public:
    explicit c_userController(QObject *parent = nullptr);
    c_userController(QMap<QString, QVariant> properties, QObject *parent = nullptr);

    QMap<QString, QVariant> getUserProperties();
    QMap<QString, QVariant> getEmployeeProperties();


    const QList<myStructures::myLog> &getDbLogs() const;
    void setDbLogs(const QList<myStructures::myLog> &newDbLogs);
    void setDbLogs(QList<QMap<QString, QVariant>> logs);

public slots:
    void setProperties(QMap<QString, QVariant> userInfo);


private:
    w_logsWindow *logs;

    QList<myStructures::myLog> dbLogs;

signals:
    void propertiesSaved();
    void passProperties(QMap<QString, QVariant> userInfo);
    void logsSaved();
    void passLogs(QList<myStructures::myLog> logs);
};

#endif // C_USERCONTROLLER_H

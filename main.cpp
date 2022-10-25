#include "w_mainwindow.h"
#include "w_logswindow.h"
#include "c_parser.h"
#include "c_moduleapplication.h"
#include "c_clientprocessconnection.h"

#include <QApplication>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QByteArray>
#include <QObject>

int main(int argc, char *argv[])
{
    QMap<QString, QVariant> args = c_Parser::ParseArguments(argc, argv);
    c_moduleApplication a(argc, argv);


    {
    if(args["ServerName"].toByteArray().isEmpty()) {
        QMessageBox::critical(nullptr, QString("Błąd uruchamiania."),
                                       QString("Uruchom aplikację za pomocą klienta: Clinic Client"),
                                       QMessageBox::Ok, QMessageBox::Ok);
        return 0xff01;
    }
    if(args["ModuleName"].toByteArray().isEmpty()) {
        QMessageBox::critical(nullptr, QString("Błąd uruchamiania."),
                                       QString("Uruchom aplikację za pomocą klienta: Clinic Client"),
                                       QMessageBox::Ok, QMessageBox::Ok);
        return 0xff02;
    }
    if(args["UserId"].toByteArray().isEmpty() || args["UserName"].toByteArray().isEmpty()) {
        QMessageBox::critical(nullptr, QString("Błąd uruchamiania."),
                                       QString("Uruchom aplikację za pomocą klienta: Clinic Client"),
                                       QMessageBox::Ok, QMessageBox::Ok);
        return 0xff03;
    }
    }


    w_MainWindow w;
    w_logsWindow * logsWindow = w_logsWindow::Instance();

    c_clientProcessConnection * connection = new c_clientProcessConnection( QByteArray::fromHex( args["ServerName"].toByteArray() ) );
    a.setProcessIdentifier( QByteArray::fromHex( args["ModuleName"].toByteArray() ));
    a.setThreadIdentifier( args["ThreadId"].toInt() );
    connection->setLogsWindow(logsWindow);

    QString log = QString("Otrzymane argumenty: \n");
    foreach (const QString key, args.keys()) {
        log += QString("%1:\t%2\n").arg(key, args[key].toString());
    }
    logsWindow->addLog(log);

    connection->establishConnection();

    QObject::connect(&a, SIGNAL(aboutToQuit()), connection, SLOT(deleteLater()));
    QObject::connect(&a, SIGNAL(aboutToQuit()), logsWindow, SLOT(deleteLater()));

    w.show();
    logsWindow->show();


    return a.exec();
}

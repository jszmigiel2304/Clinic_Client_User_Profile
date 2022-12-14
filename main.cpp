#include "w_mainwindow.h"
#include "c_parser.h"
#include "c_moduleapplication.h"
#include "c_clientprocessconnection.h"
#include "c_modulecontroller.h"
#include "c_usercontroller.h"

#include <QApplication>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QByteArray>
#include <QObject>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QMap<QString, QVariant> args = c_Parser::ParseArguments(argc, argv);
    c_moduleApplication a(argc, argv);

    //sprawdzenie poprawności otrzymanych argumentów z Clinic Client
    //użytkownik musi być zalogowany, inaczej zamknij aplikację
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
    //---------------------------------------------------------------



    a.setProcessIdentifier( QByteArray::fromHex( args["ModuleName"].toByteArray() ));
    a.setThreadIdentifier( args["ThreadId"].toInt() );

    c_moduleController * moduleCtrlr = new c_moduleController(QByteArray::fromHex(args["ServerName"].toByteArray()),
                                                            QByteArray::fromHex(args["ModuleName"].toByteArray()),
                                                            args["ThreadId"].toInt());

    moduleCtrlr->getMainWnd()->show();
    moduleCtrlr->setUserCtrlr( new c_userController(args) );
    moduleCtrlr->connectToLocalServer();


    QObject::connect(&a, SIGNAL(aboutToQuit()), moduleCtrlr, SLOT(deleteLater()));

    return a.exec();
}

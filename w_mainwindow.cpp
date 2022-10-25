#include "w_mainwindow.h"
#include "ui_w_mainwindow.h"

w_MainWindow::w_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::w_MainWindow)
{
    ui->setupUi(this);
}

w_MainWindow::~w_MainWindow()
{
    delete ui;
}


w_moreLogsWindow::w_moreLogsWindow(QList<myStructures::myLog> *Logs, QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowModality(Qt::NonModal);
    setWindowTitle(QString("Clinic Clinet - more logs"));
    setStyleSheet(QString("background-color: rgb(221, 221, 221);\ncolor: rgb(0,0,0);"));
    setGeometry(0,0,450,700);
    setMinimumSize(450,700);
    setMaximumSize(450,700);

    central.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));
    this->setCentralWidget(&central);
    setGeometry(0,0,450,690);

    scrollArea.setParent(&central);
    scrollArea.setGeometry(0,0,450,690);
    scrollArea.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));
    scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scrollArea.setWidget(&scrollAreaWidget);
    scrollAreaWidget.setGeometry(0,0,430, 25 * Logs->size() + 20);
    scrollAreaWidget.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));

    for( int i = 0; i < Logs->size(); i++) {
        QLabel * l_log = new QLabel(&scrollAreaWidget);
        l_log->setStyleSheet( QString("color: rgb(221, 221, 221);") );
        l_log->setGeometry( 0, 25 * (i + 1), 450, 25 );
        l_log->setIndent(5);
        l_log->setText( QString("%4: %2\t%3\t[ %1 ]").arg( (*Logs)[i].log_text, (*Logs)[i].time.toString(), (*Logs)[i].ip_address.toString(), QString("%1").arg(i+1) ) );
        l_log->show();
    }

    central.show();
    scrollArea.show();
    scrollAreaWidget.show();
}

w_moreLogsWindow::~w_moreLogsWindow()
{

}

#ifndef W_MAINWINDOW_H
#define W_MAINWINDOW_H

#include "c_myStructures.h"
#include "c_waitingloop.h"
#include "w_logswindow.h"

#include <QMainWindow>
#include <QScrollArea>
#include <QMap>
#include <QVariant>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class w_MainWindow; }
QT_END_NAMESPACE

class w_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    w_MainWindow(QWidget *parent = nullptr);
    ~w_MainWindow();

    QList<myStructures::myLog> *getLogs();
    QMap<QString, QVariant> *getUserProperties();
    QMap<QString, QVariant> *getEmployeeProperties();

public slots:
    void refresh();
    void setLogs(QList<myStructures::myLog> newLogs);
    void setUserProperties(QMap<QString, QVariant> newUserProperties);
    void setEmployeeProperties(QMap<QString, QVariant> newEmployeeProperties);

    void processing(QString text);
    void processingFinished(int code);
    void processingFault(QString text);

private:
    Ui::w_MainWindow *ui;
    QList<myStructures::myLog> Logs;
    QMap<QString, QVariant> userProperties;
    QMap<QString, QVariant> employeeProperties;

    w_logsWindow* logsWnd;


private slots:
    void moreLogsButtonClicked(bool checked);
    void refreshUserInfo();
    void refreshEmployeeInfo();
    void refreshLogs();
};

class w_moreLogsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_moreLogsWindow(QList<myStructures::myLog> * Logs, QWidget *parent = nullptr);
    ~w_moreLogsWindow();


private:
    QWidget central;
    QScrollArea scrollArea;
    QWidget scrollAreaWidget;


};
#endif // W_MAINWINDOW_H

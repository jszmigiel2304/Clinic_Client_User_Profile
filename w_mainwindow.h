#ifndef W_MAINWINDOW_H
#define W_MAINWINDOW_H

#include "c_myStructures.h"

#include <QMainWindow>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class w_MainWindow; }
QT_END_NAMESPACE

class w_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    w_MainWindow(QWidget *parent = nullptr);
    ~w_MainWindow();

private:
    Ui::w_MainWindow *ui;
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

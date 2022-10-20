#include "w_mainwindow.h"
#include "w_logswindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w_MainWindow w;
    w_logsWindow * logsWindow = w_logsWindow::Instance();


    w.show();
    logsWindow->show();


    return a.exec();
}

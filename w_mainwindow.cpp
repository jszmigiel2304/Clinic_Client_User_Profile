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


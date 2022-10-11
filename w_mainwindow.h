#ifndef W_MAINWINDOW_H
#define W_MAINWINDOW_H

#include <QMainWindow>

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
#endif // W_MAINWINDOW_H

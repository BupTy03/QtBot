#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "addtaskwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ExitAction_triggered();

    void on_NewTaskAction_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

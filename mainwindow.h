#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QMessageBox>

#include "addtaskwindow.h"
#include "logintovk.h"
#include "logindialog.h"
#include "myvk.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ExitAction_triggered();

    void on_NewTaskAction_triggered();

    void on_LoginAction_triggered();

    void slotGroupsLoaded();

private:
    Ui::MainWindow *ui{nullptr};
    const QString app_id{"6667132"};
    MyVK* myVK{nullptr};
};

#endif // MAINWINDOW_H

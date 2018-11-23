#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QMessageBox>

#include "addtaskwindow.h"
#include "task.h"
#include "taskwidget.h"
#include "vkauth.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_ExitAction_triggered();

    void on_NewTaskAction_triggered();

    void on_LoginAction_triggered();

    void checkLogin(bool success);

private:
    Ui::MainWindow *ui;
    VKAuth* vkAuth_;
    const QString app_id_{"6667132"};
    const QString scope_{"270336"}; //   331776   266240
    QVector<QPair<QString, QString>> groups_;
    QThread* secondThread_;
};

#endif // MAINWINDOW_H

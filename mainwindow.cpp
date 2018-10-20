#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("QtBot"));

    connect(ui->LoginBtn, SIGNAL(released()), ui->LoginAction, SLOT(trigger()));
    //connect(ui->NewTaskBtn, SIGNAL(released()), ui->NewTaskAction, SLOT(trigger()));
    connect(ui->ExitBtn, SIGNAL(released()), ui->ExitAction, SLOT(trigger()));

    ui->NewTaskBtn->hide();
    ui->NewTaskAction->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ExitAction_triggered()
{
    QApplication::quit();
}

void MainWindow::on_NewTaskAction_triggered()
{
    AddTaskWindow addTskWin;
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

    ui->statusBar->showMessage(QString("Interval: ") + QString::number(addTskWin.getInterval()) +
                               QString("\tPeriod: ") + QString::number(addTskWin.getPeriod()));
}

void MainWindow::on_LoginAction_triggered()
{
   LoginDialog logDial(app_id);
   logDial.exec();
   this->user_info = logDial.getInfo();
   if(user_info.first.isEmpty() || user_info.second.isEmpty())
   {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось войти ¯\\_(ツ)_/¯"));
        return;
   }

   //MyVK vk(user_info.second, user_info.first);
   myVK = new MyVK(user_info.second, user_info.first, this);
   myVK->sendMsgToUser(user_info.second, "Hi there! I'm QtBot and I cost much more than 2 rubles!");

   connect(ui->NewTaskBtn, SIGNAL(released()), ui->NewTaskAction, SLOT(trigger()));

   ui->LoginBtn->hide();
   ui->NewTaskBtn->show();
   ui->LoginAction->setDisabled(true);
   ui->NewTaskAction->setEnabled(true);

   ui->statusBar->showMessage(QString("Access_token: ") + user_info.first + QString("\tUser_id: ") + user_info.second);
}

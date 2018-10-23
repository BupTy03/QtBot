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
    AddTaskWindow addTskWin(myVK->getGroupsResponse());
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

    qDebug() << "====================Chosen groups======================";
    for(auto indx : addTskWin.getGroupsIndexesSet())
    {
        qDebug() << "Id of group: " << ((myVK->getGroupsResponse()).at(indx)).first;
        qDebug() << "Name of group: " << ((myVK->getGroupsResponse()).at(indx)).second << '\n';
    }
    qDebug() << "=================End of chosen groups==================";

    ui->statusBar->showMessage(QString("Interval: ") + QString::number(addTskWin.getInterval()) +
                               QString("\tPeriod: ") + QString::number(addTskWin.getPeriod()));
}

void MainWindow::on_LoginAction_triggered()
{
   LoginDialog logDial(app_id);
   logDial.exec();

   QPair<QString, QString> user_info = logDial.getInfo();
   if(user_info.first.isEmpty() || user_info.second.isEmpty())
   {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось войти ¯\\_(ツ)_/¯"));
        return;
   }

   myVK = new MyVK(user_info.first, user_info.second, this);
   //myVK->sendMsgToUserQuery(user_info.first, "Hi there! I'm QtBot and I cost much more than 2 rubles!");

   connect(myVK, SIGNAL(signalGroupsLoaded()), this, SLOT(slotGroupsLoaded()));
   myVK->getGroupsQuery(user_info.first);

   ui->statusBar->showMessage(QString("\tUser_id: ") + user_info.first + QString("Access_token: ") + user_info.second);
}

void MainWindow::slotGroupsLoaded()
{
    if((myVK->getGroupsResponse()).empty())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось загрузить список сообществ :("));
        return;
    }

    connect(ui->NewTaskBtn, SIGNAL(released()), ui->NewTaskAction, SLOT(trigger()));

    ui->LoginBtn->hide();
    ui->NewTaskBtn->show();
    ui->LoginAction->setDisabled(true);
    ui->NewTaskAction->setEnabled(true);

//    for(const auto& p : myVK->getGroupsResponse())
//    {
//        qDebug() << "Id of Group: " << p.first;
//        qDebug() << "Name of Group: " << p.second;
//    }
}

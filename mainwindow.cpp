#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <algorithm>

#include "queries_to_vk.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("QtBot"));

    vkAuth_ = new VKAuth(app_id_, this);

    QFile file("users.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    users_ = (QJsonDocument::fromJson(file.readAll())).array();
    file.close();

    std::for_each(users_.constBegin(), users_.constEnd(), [this](auto item){
        ui->ChangeUserCB->addItem((item["name"]).toString());
    });

    bool is_empty = users_.empty();

    currentUser = (is_empty) ? -1 : 0;

    ui->NewTaskBtn->setVisible(!is_empty);
    ui->NewTaskAction->setDisabled(is_empty);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget* scrollWidget = new QWidget;
    scrollWidget->setLayout(new QVBoxLayout);
    (scrollWidget->layout())->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(scrollWidget);

    QObject::connect(ui->AddUserPB, &QPushButton::released, ui->AddUser, &QAction::trigger);
    QObject::connect(ui->ExitBtn, &QPushButton::released, ui->ExitAction, &QAction::trigger);
    QObject::connect(ui->NewTaskBtn, &QPushButton::released, ui->NewTaskAction, &QAction::trigger);

    QObject::connect(vkAuth_, &VKAuth::done, this, &MainWindow::checkLogin);

    secondThread_ = new QThread(this);
    secondThread_->start();
}

MainWindow::~MainWindow()
{
    secondThread_->terminate();
    delete ui;
}

void MainWindow::on_ExitAction_triggered()
{
    QApplication::quit();
}

void MainWindow::on_NewTaskAction_triggered()
{
    if(currentUser < 0)
    {
        return;
    }

    AddTaskWindow addTskWin((users_.at(currentUser))["access_token"].toString(),
            QString::number((users_.at(currentUser))["id"].toInt()) /*vkAuth_->get_access_token(), vkAuth_->get_user_id()*/);
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

    Task* curr_task = new Task(vkAuth_->get_access_token(),
                               addTskWin.getGroupsIds(),
                               addTskWin.getMessage(),
                               addTskWin.getInterval(),
                               addTskWin.getPeriod());

    TaskWidget* widget = new TaskWidget(curr_task, addTskWin.getGroupsNames());

    (((ui->scrollArea)->widget())->layout())->addWidget(widget);

    curr_task->moveToThread(secondThread_);
}

void MainWindow::checkLogin(bool success)
{
    if(!success)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось авторизоваться ¯\\_(ツ)_/¯"));
        return;
    }

    ui->NewTaskBtn->show();
    ui->NewTaskAction->setEnabled(true);
}

void MainWindow::on_AddUser_triggered()
{
    vkAuth_->reauth(scope_);
}

void MainWindow::on_ChangeUserCB_currentIndexChanged(int index)
{
    currentUser = index;
}

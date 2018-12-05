#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QtWidgets>
#include <QAction>
#include <QMessageBox>
#include <QJsonObject>
#include <QFile>

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

    updateUsersComboBox();

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
    close();
}

void MainWindow::on_NewTaskAction_triggered()
{
    if(currentUser < 0)
    {
        return;
    }

    AddTaskWindow addTskWin((users_.at(currentUser))["access_token"].toString(),
            QString::number((users_.at(currentUser))["id"].toInt()));
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

    Task* curr_task = new Task(((users_.at(currentUser))["access_token"]).toString(),
                               addTskWin.getGroupsIds(),
                               addTskWin.getMessage(),
                               addTskWin.getInterval(),
                               addTskWin.getPeriod());

    if(addTskWin.hasImage())
    {
        curr_task->setImgPath(addTskWin.getImgPath());
    }

    TaskWidget* widget = new TaskWidget(curr_task, addTskWin.getGroupsNames());

    (((ui->scrollArea)->widget())->layout())->addWidget(widget);

    curr_task->go();
    curr_task->moveToThread(secondThread_);
}

void MainWindow::checkLogin(bool success)
{
    if(!success)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось авторизоваться ¯\\_(ツ)_/¯"));
        return;
    }

    addNewUser(vkAuth_->get_user_id(), vkAuth_->get_access_token());
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

QString MainWindow::userNameFromJson(const QJsonDocument& doc) const
{
    qDebug() << doc;
    QJsonArray arr = doc["response"].toArray();
    if(arr.empty())
    {
        return QString();
    }

    QJsonObject usr = (arr.first()).toObject();

    return (((usr["first_name"]).toString()).append(" ")).append(usr["last_name"].toString());
}

void MainWindow::addNewUser(const QString& id, const QString& access_token)
{
    QJsonObject usr;
    usr["access_token"] = access_token;
    usr["id"] = id.toInt();
    usr["name"] = userNameFromJson(VkQuery::getUserName(vkAuth_->get_user_id(), vkAuth_->get_access_token()));
    users_.append(usr);
    updateUsersComboBox();
}

void MainWindow::updateUsersComboBox()
{
    ui->ChangeUserCB->clear();
    std::for_each(users_.constBegin(), users_.constEnd(),
    [this](auto item)
    {
        ui->ChangeUserCB->addItem((item["name"]).toString());
    });
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QFile file("users.json");
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    file.write((QJsonDocument(users_)).toJson());
    file.close();
    QApplication::quit();
}

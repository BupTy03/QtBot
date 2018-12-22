#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QtWidgets>
#include <QAction>
#include <QMessageBox>
#include <QJsonObject>
#include <QFile>

#include <algorithm>
#include <exception>

#include "queries_to_vk.h"
#include "updatetokenwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("QtBot"));

    vkAuth_ = new VKAuth(app_id_, this);

    QFile file(QApplication::applicationDirPath() + "/users.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    users_ = (QJsonDocument::fromJson(file.readAll())).array();
    file.close();

    updateUsersComboBox();

    bool is_empty = users_.empty();

    currentUser = (is_empty) ? -1 : 0;

    ui->NewTaskBtn->setVisible(!is_empty);
    ui->NewTaskAction->setDisabled(is_empty);

    ui->changeTokenBtn->setVisible(!is_empty);
    ui->ChangeTokenAction->setDisabled(is_empty);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget* scrollWidget = new QWidget;
    scrollWidget->setLayout(new QVBoxLayout);
    (scrollWidget->layout())->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(scrollWidget);

    QObject::connect(ui->AddUserPB, &QPushButton::released, ui->AddUser, &QAction::trigger);
    QObject::connect(ui->ExitBtn, &QPushButton::released, ui->ExitAction, &QAction::trigger);
    QObject::connect(ui->NewTaskBtn, &QPushButton::released, ui->NewTaskAction, &QAction::trigger);
    QObject::connect(ui->changeTokenBtn, &QPushButton::released, ui->ChangeTokenAction, &QAction::trigger);

    QObject::connect(vkAuth_, &VKAuth::done, this, &MainWindow::checkLogin);

    secondThread_ = new QThread(this);
    secondThread_->start();
}

MainWindow::~MainWindow()
{
    secondThread_->quit();
    secondThread_->wait();
    delete ui;
}

void MainWindow::on_ExitAction_triggered()
{
    this->closeEvent(new QCloseEvent);
}

void MainWindow::on_NewTaskAction_triggered()
{
    if(currentUser < 0 || currentUser >= users_.size())
    {
        return;
    }

    AddTaskWindow addTskWin((((users_.at(currentUser)).toObject()).take("access_token")).toString(),
            QString::number((((users_.at(currentUser)).toObject()).take("id")).toInt()));
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        qCritical() << "Failed to add a new task!";
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Ошибка добавления задачи!"));
        return;
    }

    auto access_token = (((users_.at(currentUser)).toObject()).take("access_token")).toString();
    Task* curr_task = new Task(access_token,
                               addTskWin.getGroups(),
                               addTskWin.getMessage(),
                               addTskWin.getInterval(),
                               addTskWin.getPeriod());

    QObject::connect(this, &MainWindow::startTask,
                     curr_task, &Task::go);

    if(addTskWin.hasImage())
    {
        if(!curr_task->attachPhoto(addTskWin.getImgPath()))
        {
            qWarning() << "Failed to attach image!";
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Не удалось добавить изображение!"));
        }
    }

    (((ui->scrollArea)->widget())->layout())
            ->addWidget(new TaskWidget(curr_task));

    curr_task->moveToThread(secondThread_);
    emit startTask();
}

void MainWindow::checkLogin(bool success)
{
    if(!success)
    {
        qCritical() << "Failed to log in!";
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Не удалось авторизоваться ¯\\_(ツ)_/¯"));
        return;
    }

    addNewUser(vkAuth_->get_user_id(), vkAuth_->get_access_token());
    ui->NewTaskBtn->show();
    ui->NewTaskAction->setEnabled(true);
    ui->changeTokenBtn->show();
    ui->ChangeTokenAction->setEnabled(true);
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
    QJsonArray arr = (doc.object())["response"].toArray();
    if(arr.empty())
    {
        return QString();
    }

    QJsonObject usr = (arr.first()).toObject();

    return ((usr["first_name"].toString())
            .append(" "))
            .append(usr["last_name"].toString());
}

void MainWindow::addNewUser(const QString& id, const QString& access_token)
{
    QJsonObject usr;
    usr["access_token"] = access_token;
    usr["id"] = id.toInt();

    try
    {
        usr["name"] = userNameFromJson(
                    VkQuery::getUserName(vkAuth_->get_user_id(),
                                         vkAuth_->get_access_token()));
    }
    catch (const std::exception& e)
    {
        usr["name"] = id;
        qWarning() << "Failed to load name of user error: " << e.what();
    }

    users_.append(usr);
    updateUsersComboBox();
}

void MainWindow::updateUsersComboBox()
{
    ui->ChangeUserCB->clear();
    std::for_each(users_.constBegin(), users_.constEnd(),
    [this](const QJsonValue& item)
    {
        ui->ChangeUserCB->addItem((item.toObject())["name"].toString());
    });
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QFile file(QApplication::applicationDirPath() + "/users.json");
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Failed to save users.json file!";
        QMessageBox::warning(this,
                              tr("Предупреждение"),
                              tr("Не удалось записать пользователя в файл users.json"));
        return;
    }
    file.write(QJsonDocument(users_).toJson());
    file.close();
    QApplication::quit();
}

void MainWindow::on_ChangeTokenAction_triggered()
{
    if(currentUser < 0 || currentUser >= users_.size())
    {
        return;
    }

    UpdateTokenWidget updateTWin;

    if(updateTWin.exec() != QDialog::Accepted)
    {
        return;
    }

    QJsonObject tmp_user = (users_.at(currentUser)).toObject();
    tmp_user["access_token"] = updateTWin.getToken();
    users_[currentUser] = tmp_user;
    QMessageBox::information(this,
                             tr("Успех"),
                             tr("Вы успешно сменили токен!"));
}

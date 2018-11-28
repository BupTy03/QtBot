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

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget* scrollWidget = new QWidget;
    scrollWidget->setLayout(new QVBoxLayout);
    (scrollWidget->layout())->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(scrollWidget);

    QObject::connect(ui->LoginBtn, &QPushButton::released, ui->LoginAction, &QAction::trigger);
    QObject::connect(ui->ExitBtn, &QPushButton::released, ui->ExitAction, &QAction::trigger);
    QObject::connect(ui->NewTaskBtn, &QPushButton::released, ui->NewTaskAction, &QAction::trigger);

    QObject::connect(vkAuth_, &VKAuth::done, this, &MainWindow::checkLogin);

    ui->NewTaskBtn->hide();
    ui->NewTaskAction->setDisabled(true);
    ui->TokenAction->setDisabled(true);

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
    QStringList groups_names_;
    groups_names_.reserve(groups_.size());

    std::transform(groups_.cbegin(), groups_.cend(), std::back_inserter(groups_names_),
    [](const auto& p){
        return p.second;
    });

    AddTaskWindow addTskWin(vkAuth_->get_access_token(), vkAuth_->get_user_id());
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

//    auto groupsIndexes = addTskWin.getGroupsIndexes();
//    QStringList groups_names;
//    QStringList groups_ids;

//    groups_names.reserve(groupsIndexes.size());
//    groups_ids.reserve(groupsIndexes.size());

//    for(auto index : groupsIndexes)
//    {
//        const auto& group = groups_.at(index);
//        groups_names.push_back(group.second);
//        groups_ids.push_back(group.first);
//    }

    Task* curr_task = new Task(vkAuth_->get_access_token(),
                               addTskWin.getGroupsIds(),
                               addTskWin.getMessage(),
                               addTskWin.getInterval(),
                               addTskWin.getPeriod());

    TaskWidget* widget = new TaskWidget(curr_task, addTskWin.getGroupsNames());

    (((ui->scrollArea)->widget())->layout())->addWidget(widget);

    curr_task->moveToThread(secondThread_);
}

void MainWindow::on_LoginAction_triggered()
{
    ui->LoginAction->setDisabled(true);
    ui->LoginBtn->setDisabled(true);
    vkAuth_->auth(scope_);
    ui->TokenAction->setEnabled(true);
}

void MainWindow::checkLogin(bool success)
{
    if(!success)
    {
        ui->LoginAction->setEnabled(true);
        ui->LoginBtn->setEnabled(true);
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось авторизоваться ¯\\_(ツ)_/¯"));
        return;
    }
    ui->LoginBtn->hide();

    ui->NewTaskBtn->show();
    ui->NewTaskAction->setEnabled(true);

    //groups_ = get_groups_from_json(vk_query::groups_get(vkAuth_->get_access_token(), vkAuth_->get_user_id()));

#if 0
    qDebug() << "\n\n===================================Groups list=====================================";
    std::transform(std::cbegin(groups_), std::cend(groups_), std::ostream_iterator<std::string>(std::cout, "\n"),
    [](const QPair<QString, QString>& p)
    {
         QString result;
         result.push_back("Id of group: ");
         result.push_back(p.first);
         result.push_back("\nName of group: ");
         result.push_back(p.second);
         return result.toStdString();
    });
    qDebug() << "===================================End of list=====================================";
#endif
}

void MainWindow::on_TokenAction_triggered()
{
    vkAuth_->reauth(scope_);
}

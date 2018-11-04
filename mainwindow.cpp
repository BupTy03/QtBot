#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <algorithm>

#include "queries_to_vk.h"

QVector<QPair<QString, QString>> get_groups_from_json(const QJsonDocument& document)
{
    QJsonObject response = (document.object())["response"].toObject();

    int countGr = response["count"].toInt();

    if(countGr <= 0)
    {
        return QVector<QPair<QString, QString>>();
    }

    QVector<QPair<QString, QString>> groups;
    groups.reserve(countGr);

    QJsonArray items = response["items"].toArray();

    for(auto gr : items)
    {
        QJsonObject tmpGroup = gr.toObject();

        groups.push_back(qMakePair(
         QString::number(tmpGroup["id"].toInt()),
                         tmpGroup["name"].toString()));
    }
    return groups;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pauseThread_(false)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("QtBot"));

    vkAuth_ = new VKAuth(app_id_, this);

    tasks_layout_ = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setMinimumHeight(50);
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    ui->scrollAreaWidgetContents->setLayout(tasks_layout_);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(ui->LoginBtn, &QPushButton::released, ui->LoginAction, &QAction::trigger);
    connect(ui->ExitBtn, &QPushButton::released, ui->ExitAction, &QAction::trigger);
    connect(ui->NewTaskBtn, &QPushButton::released, ui->NewTaskAction, &QAction::trigger);

    ui->NewTaskBtn->hide();
    ui->NewTaskAction->setDisabled(true);

    thread_ = std::thread(
    [this]()
    {
        std::mutex lock_thread;
        while(true)
        {
            std::lock_guard<std::mutex> locker(lock_thread);

            tasks_.erase(std::remove_if(std::begin(tasks_), std::end(tasks_), [](Task* t){ return t->isRemoved(); }), tasks_.end());

            for(auto task : tasks_)
            {
                task->run();
            }
        }
    });
    thread_.detach();
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
    QStringList names_of_gr;
    std::transform(std::cbegin(groups_), std::cend(groups_), std::back_inserter(names_of_gr),
    [](const auto& p)
    {
        return p.second;
    });

    AddTaskWindow addTskWin(names_of_gr);
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

    ui->scrollAreaWidgetContents->setMinimumHeight(ui->scrollAreaWidgetContents->minimumHeight() + 200);

    auto groupsIndexes = addTskWin.getGroupsIndexes();
    QStringList groups_names;
    QStringList groups_ids;

    groups_names.reserve(groupsIndexes.size());
    groups_ids.reserve(groupsIndexes.size());

    for(auto index : groupsIndexes)
    {
        const auto& group = groups_.at(index);
        groups_names.push_back(group.second);
        groups_ids.push_back(group.first);
    }

    Task* curr_task = new Task(vkAuth_->get_access_token(),
                               groups_ids,
                               addTskWin.getMessage(),
                               addTskWin.getInterval(),
                               addTskWin.getPeriod(),
                               this);

    TaskWidget* widget = new TaskWidget(curr_task, groups_names);
    tasks_layout_->addWidget(widget);

    addNewTask(curr_task);
}

void MainWindow::on_LoginAction_triggered()
{
    ui->LoginAction->setDisabled(true);
    ui->LoginBtn->setDisabled(true);

    vkAuth_->auth(scope_);
    connect(vkAuth_, &VKAuth::done, [this](bool success)
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

        groups_ = get_groups_from_json(vk_query::groups_get(vkAuth_->get_access_token(), vkAuth_->get_user_id()));

#ifdef DEBUG
        qDebug() << "\n\n===================================Groups list=====================================";
        std::transform(std::cbegin(groups_), std::cend(groups_), std::ostream_iterator<std::string>(std::cout, "\n"), [](const QPair<QString, QString>& p)
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
    });
}

void MainWindow::addNewTask(Task* tsk)
{
    std::lock_guard<std::mutex> locker(lock_);
    tasks_.push_back(tsk);
}

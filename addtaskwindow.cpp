#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

AddTaskWindow::AddTaskWindow(const QVector<QPair<QString, QString>>& groups, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));

    ui->intervalSpinBox->setMaximum(999);
    ui->periodSpinBox->setMaximum(3600);

    groupsModel = new GroupsChoiceModel(groups, this);

    ui->listView->setModel(groupsModel);
}

AddTaskWindow::~AddTaskWindow()
{
    delete ui;
}

int AddTaskWindow::getInterval()
{
    return ui->intervalSpinBox->value();
}

int AddTaskWindow::getPeriod()
{
    return ui->periodSpinBox->value();
}

const QSet<int>& AddTaskWindow::getGroupsIndexesSet()
{
    return groupsModel->chosenGroupsSet();
}

QString AddTaskWindow::getMessage()
{
    return ui->textMessage->toPlainText();
}

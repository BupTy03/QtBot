#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

AddTaskWindow::AddTaskWindow(const QVector<QPair<QString, QString>>& groups, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));

    ui->intervalSpinBox->setRange(0, 7200000);
    ui->periodSpinBox->setRange(0, 18000);

    groupsModel_ = new QStandardItemModel(this);

    for(const auto& group : groups)
    {
        QStandardItem* tmp = new QStandardItem(group.second);
        tmp->setCheckable(true);
        tmp->setEditable(false);
        groupsModel_->appendRow(tmp);
    }

    ui->listView->setModel(groupsModel_);
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

QVector<int> AddTaskWindow::getGroupsIndexes()
{   
    QVector<int> indexes;
    const auto rcount = groupsModel_->rowCount();
    for(int i = 0; i < rcount; ++i)
    {
        if((groupsModel_->item(i, 0))->checkState() == Qt::Checked)
        {
            indexes.push_back(i);
        }
    }
    return indexes;
}

QString AddTaskWindow::getMessage() const
{
    return ui->textMessage->toPlainText();
}

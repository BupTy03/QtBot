#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

AddTaskWindow::AddTaskWindow(const QStringList& groups, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));

    ui->intervalSpinBox->setRange(0, 7200000);
    ui->periodSpinBox->setRange(0, 18000);

    groupsModel_ = new ChoiceListModel(groups, this);

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
    return groupsModel_->getChosenElems();
}

QString AddTaskWindow::getMessage() const
{
    return ui->textMessage->toPlainText();
}

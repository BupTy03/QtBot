#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

AddTaskWindow::AddTaskWindow(const QStringList& groups, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow),
    userGroups_(groups)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));
    this->setWindowFlags (this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->intervalSpinBox->setRange(1, 10000);
    ui->periodSpinBox->setRange(1, 18000);

    groupsModel_ = new QStandardItemModel(this);

    ui->listView->setModel(groupsModel_);
    ui->radioBtnList->toggle();
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

QVector<int> AddTaskWindow::getGroupsIndexes() const
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

QStringList AddTaskWindow::getGroups() const
{
    QVector<int> indexes = getGroupsIndexes();
    QStringList result;
    result.reserve(indexes.size());

    std::transform(indexes.cbegin(), indexes.cend(), std::back_inserter(result),
    [this](int indx){
        return userGroups_.at(indx);
    });

    return result;
}

QString AddTaskWindow::getMessage() const
{
    return ui->textMessage->toPlainText();
}

void AddTaskWindow::on_radioBtnList_toggled(bool checked)
{
    if(!checked)
    {
        return;
    }

    ui->buttonBox->setEnabled(false);
    groupsModel_->clear();

    currentList_ = userGroups_;

    for(const auto& group : currentList_)
    {
        QStandardItem* tmp = new QStandardItem(group);
        tmp->setCheckable(true);
        tmp->setEditable(false);
        groupsModel_->appendRow(tmp);
    }

    ui->buttonBox->setEnabled(true);
}

void AddTaskWindow::on_radioBtnFile_toggled(bool checked)
{
    if(!checked)
    {
        return;
    }

    this->setDisabled(true);

    groupsModel_->clear();

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open text file"), QDir::currentPath(), tr("Text files (*.txt)"));

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось открыть файл!"));
        this->close();
    }

    currentList_ = QString(file.readAll()).split("\r\n");

    for(const auto& group : currentList_)
    {
        QStandardItem* tmp = new QStandardItem(group);
        tmp->setCheckable(true);
        tmp->setEditable(false);
        tmp->setCheckState(Qt::Checked);
        groupsModel_->appendRow(tmp);
    }

    this->setEnabled(true);
}

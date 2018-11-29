#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"
#include "queries_to_vk.h"

QVector<std::shared_ptr<Group>> get_groups_from_json(const QJsonDocument& document)
{
    QJsonObject response = (document.object())["response"].toObject();

    const auto countGr = response["count"].toInt();

    QVector<std::shared_ptr<Group>> groups;
    groups.reserve(countGr);

    QJsonArray items = response["items"].toArray();

    for(auto gr : items)
    {
        QJsonObject tmpGroup = gr.toObject();

        groups.push_back(std::make_shared<Group>(
                         QString::number(tmpGroup["id"].toInt()),
                         tmpGroup["name"].toString()
                        ));
    }
    return groups;
}

AddTaskWindow::AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));
    this->setWindowFlags (this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->intervalSpinBox->setRange(1, 10000);
    ui->periodSpinBox->setRange(1, 18000);

    groupsModel_ = new QStandardItemModel(this);

    userGroups_ = get_groups_from_json(vk_query::groups_get(access_token, user_id));

    ui->listView->setModel(groupsModel_);
    ui->radioBtnList->toggle();
}

AddTaskWindow::~AddTaskWindow()
{
    delete ui;
}

int AddTaskWindow::getInterval() const
{
    return ui->intervalSpinBox->value();
}

int AddTaskWindow::getPeriod() const
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

QStringList AddTaskWindow::getGroupsNames() const
{
    QVector<int> indexes = getGroupsIndexes();
    QStringList result;
    result.reserve(indexes.size());

    std::transform(indexes.cbegin(), indexes.cend(), std::back_inserter(result),
    [this](auto indx){
        return (currentList_.at(indx))->name;
    });

    return result;
}

QStringList AddTaskWindow::getGroupsIds() const
{
    QVector<int> indexes = getGroupsIndexes();
    QStringList result;
    result.reserve(indexes.size());

    std::transform(indexes.cbegin(), indexes.cend(), std::back_inserter(result),
    [this](auto indx){
        return (currentList_.at(indx))->id;
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

    updateItems();

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

    QStringList lst = QString(file.readAll()).split("\r\n");

    currentList_.clear();
    currentList_.reserve(lst.size());

    std::transform(std::cbegin(lst), std::cend(lst), std::back_inserter(currentList_),
    [](const auto& gr)
    {
        return std::make_shared<Group>(gr, gr);
    });

    updateItems();

    this->setEnabled(true);
}

void AddTaskWindow::updateItems()
{
    std::for_each(std::cbegin(currentList_), std::cend(currentList_), [this](const auto& gr)
    {
        QStandardItem* tmp = new QStandardItem(gr->name);
        tmp->setCheckable(true);
        tmp->setEditable(false);
        this->groupsModel_->appendRow(tmp);
    });
}

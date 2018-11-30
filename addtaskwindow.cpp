#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"
#include "queries_to_vk.h"

AddTaskWindow::AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow),
    userGroups_(getGroupsFromJson(vk_query::groups_get(access_token, user_id)))
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));
    this->setWindowFlags (this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->intervalSpinBox->setRange(1, 10000);
    ui->periodSpinBox->setRange(1, 18000);

    groupsModel_ = new QStandardItemModel(this);

    //userGroups_ = get_groups_from_json(vk_query::groups_get(access_token, user_id));

    ui->listView->setModel(groupsModel_);
    if(userGroups_.empty())
    {
        ui->radioBtnList->setVisible(false);
        ui->radioBtnFile->toggle();
        return;
    }
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

std::vector<int> AddTaskWindow::getGroupsIndexes() const
{   
    std::vector<int> indexes;
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
    std::vector<int> indexes = getGroupsIndexes();
    QStringList result;
    result.reserve(static_cast<QStringList::size_type>(indexes.size()));

    std::transform(indexes.cbegin(), indexes.cend(), std::back_inserter(result),
    [this](auto indx){
        return (currentList_.at(indx)).name;
    });

    return result;
}

QStringList AddTaskWindow::getGroupsIds() const
{
    std::vector<int> indexes = getGroupsIndexes();
    QStringList result;
    result.reserve(static_cast<QStringList::size_type>(indexes.size()));

    std::transform(indexes.cbegin(), indexes.cend(), std::back_inserter(result),
    [this](auto indx){
        return (currentList_.at(indx)).id;
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

    currentList_ = userGroups_;

    updateItems();
}

void AddTaskWindow::on_radioBtnFile_toggled(bool checked)
{
    if(!checked)
    {
        return;
    }

    this->setDisabled(true);

    currentList_ = readGroupsFromFile(QFileDialog::getOpenFileName(this,
                                     tr("Open text file"), QDir::currentPath(), tr("Json files (*.json)")));

    this->setEnabled(true);

    if(currentList_.empty())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось прочитать файл!"));
        ui->radioBtnList->toggle();
        return;
    }

    updateItems();

//    QString filename = QFileDialog::getOpenFileName(this,
//        tr("Open text file"), QDir::currentPath(), tr("Text files (*.txt)"));

//    QFile file(filename);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось открыть файл!"));
//        this->close();
//    }

//    QJsonArray groupsJArray = (QJsonDocument::fromJson(file.readAll())).array();

//    std::transform(groupsJArray.constBegin(), groupsJArray.constEnd(),
//                   std::back_inserter(currentList_),
//    [](auto group)
//    {
//        return std::make_shared<Group>(group["id"], group["name"]);
//    });

//    QStringList lst = QString(file.readAll()).split("\r\n");

//    currentList_.clear();
//    currentList_.reserve(lst.size());

//    std::transform(std::cbegin(lst), std::cend(lst), std::back_inserter(currentList_),
//    [](const auto& gr)
//    {
//        return std::make_shared<Group>(gr, gr);
//    });
}

void AddTaskWindow::updateItems()
{
    this->groupsModel_->clear();
    std::for_each(std::cbegin(currentList_), std::cend(currentList_), [this](const auto& gr)
    {
        QStandardItem* tmp = new QStandardItem(gr.name);
        tmp->setCheckable(true);
        tmp->setEditable(false);
        this->groupsModel_->appendRow(tmp);
    });
}

std::vector<Group> AddTaskWindow::getGroupsFromJson(const QJsonDocument& document) const
{
    QJsonObject response = (document.object())["response"].toObject();

    std::vector<Group> groups;
    groups.reserve(static_cast<std::vector<Group>::size_type>(response["count"].toInt()));

    QJsonArray items = response["items"].toArray();

    std::transform(std::cbegin(items), std::cend(items), std::back_inserter(groups),
    [](auto json_val)
    {
        return Group(QString::number(json_val["id"].toInt()), json_val["name"].toString());
    });

    return groups;
}

std::vector<Group> AddTaskWindow::readGroupsFromFile(const QString& filename)
{
    std::vector<Group> result;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return result;
    }

    QJsonArray groupsJArray = (QJsonDocument::fromJson(file.readAll())).array();
    result.reserve(static_cast<std::vector<Group>::size_type>(groupsJArray.size()));

    std::transform(groupsJArray.constBegin(), groupsJArray.constEnd(),
                   std::back_inserter(result),
    [](auto group)
    {
        return Group(QString::number(group["id"].toInt()), group["name"].toString());
    });

    return result;
}


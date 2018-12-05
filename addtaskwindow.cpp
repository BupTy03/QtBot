#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QSize>
#include <QResizeEvent>

#include <QDebug>

#include <utility>
#include <algorithm>

#include "queries_to_vk.h"

template<typename T, typename... Ts>
QSharedPointer<T> makeQSharedPointer(Ts&&... params)
{
    return QSharedPointer<T>(new T(std::forward<Ts>(params)...));
}

AddTaskWindow::AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow),
    userGroups_(getGroupsFromJson(VkQuery::groupsGet(access_token, user_id)))
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));
    this->setWindowFlags (this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->intervalSpinBox->setRange(1, 10000);
    ui->periodSpinBox->setRange(1, 18000);

    groupsModel_ = new QStandardItemModel(this);
    ui->listView->setModel(groupsModel_);

    ui->loadImageLabel->hide();

    if(userGroups_->empty())
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
        return (currentList_->at(indx)).name;
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
        return (currentList_->at(indx)).id;
    });

    return result;
}

QString AddTaskWindow::getMessage() const
{
    return ui->textMessage->toPlainText();
}

bool AddTaskWindow::hasImage() const
{
    return !imgPath_.isEmpty();
}

QString AddTaskWindow::getImgPath() const
{
    return imgPath_;
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
                                     tr("Открыть файл JSON"), QDir::currentPath(), tr("Json files (*.json)")));

    this->setEnabled(true);

    if(currentList_->empty())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось прочитать файл!"));
        ui->radioBtnList->toggle();
        return;
    }

    updateItems();
}

void AddTaskWindow::updateItems()
{
    this->groupsModel_->clear();
    std::for_each(std::cbegin(*currentList_), std::cend(*currentList_), [this](const auto& gr)
    {
        QStandardItem* tmp = new QStandardItem(gr.name);
        tmp->setCheckable(true);
        tmp->setEditable(false);
        this->groupsModel_->appendRow(tmp);
    });
}

bool AddTaskWindow::areAllItemsUnchecked()
{
    const auto rcount = groupsModel_->rowCount();
    for(int i = 0; i < rcount; ++i)
    {
        if((groupsModel_->item(i, 0))->checkState() == Qt::Checked)
        {
            return false;
        }
    }
    return true;
}

QSharedPointer<QVector<Group>> AddTaskWindow::getGroupsFromJson(const QJsonDocument& document) const
{
    QJsonObject response = (document.object())["response"].toObject();

    auto groups = makeQSharedPointer<QVector<Group>>();
    groups->reserve(response["count"].toInt());

    QJsonArray items = response["items"].toArray();

    qDebug() << "==================Groups loaded==================";

    std::transform(std::cbegin(items), std::cend(items),
                   std::back_inserter(*groups),
    [](auto json_val)
    {
        qDebug() << "GroupID: " << QString::number(json_val["id"].toInt());
        qDebug() << "GroupName: " << json_val["name"].toString();
        qDebug();
        return Group(QString::number(json_val["id"].toInt()), json_val["name"].toString());
    });

    qDebug() << "=================================================";

    return groups;
}

QSharedPointer<QVector<Group>> AddTaskWindow::readGroupsFromFile(const QString& filename)
{
    auto groups = makeQSharedPointer<QVector<Group>>();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return groups;
    }

    QJsonArray groupsJArray = (QJsonDocument::fromJson(file.readAll())).array();
    groups->reserve(groupsJArray.size());

    std::transform(groupsJArray.constBegin(), groupsJArray.constEnd(),
                   std::back_inserter(*groups),
    [](auto group)
    {
        return Group(QString::number(group["id"].toInt()), group["name"].toString());
    });

    return groups;
}

int AddTaskWindow::exec()
{
    int result = QDialog::exec();
    return (areAllItemsUnchecked()) ? QDialog::Rejected : result;
}

void AddTaskWindow::resizeEvent(QResizeEvent* event)
{
    if(ui->loadImageLabel->isVisible() && !loadImg_.isNull())
    {
        updateLoadImg();
    }
}

void AddTaskWindow::on_loadImageBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Загрузить изображение"),
                                                    QDir::currentPath(),
                                                    tr("Images (*.jpg)"));
    if(!loadImg_.load(filename))
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка чтения файла!"));
        return;
    }

    imgPath_ = filename;

    ui->loadImageLabel->show();
    updateLoadImg();
}

void AddTaskWindow::updateLoadImg()
{
    QSize sz = ui->loadImageLabel->size();
    ui->loadImageLabel->setPixmap(loadImg_.scaled(sz.width(), sz.height(), Qt::KeepAspectRatio));
}

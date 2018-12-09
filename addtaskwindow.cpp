#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QJsonObject>
#include <QSize>
#include <QResizeEvent>
#include <QDebug>

#include <algorithm>
#include <exception>

#include "queries_to_vk.h"

AddTaskWindow::AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Добавление задачи"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->intervalSpinBox->setRange(1, 10000);
    ui->periodSpinBox->setRange(1, 18000);

    groupsModel_ = new QStandardItemModel(this);
    ui->listView->setModel(groupsModel_);

    ui->loadImageLabel->hide();

    try
    {
        userGroups_ = getGroupsFromJson(VkQuery::groupsGet(access_token, user_id));
    }
    catch(const std::exception& e)
    {
        qWarning() << "Failed to load the list of groups of user error: " << e.what();
    }

    if(!userGroups_ || userGroups_->empty())
    {
        QMessageBox::warning(this, tr("Предупреждение"),
                             tr("Не удалось загузить список сообществ пользователя"));
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

std::vector<std::pair<QString, QString>> AddTaskWindow::getGroups() const
{
    std::vector<std::pair<QString, QString>> result;
    if(!currentList_)
    {
        return result;
    }

    const auto rcount = groupsModel_->rowCount();
    for(int i = 0; i < rcount; ++i)
    {
        if((groupsModel_->item(i, 0))->checkState() == Qt::Checked)
        {
            const auto& group = (*currentList_).at(i);
            result.emplace_back(QString::number(group["id"].toInt()),
                                group["name"].toString());
        }
    }

    return result;
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
                                      tr("Загрузить сообщества из файла JSON"),
                                      QDir::currentPath(),
                                      tr("Json files (*.json)")));

    this->setEnabled(true);

    if(!currentList_ || currentList_->isEmpty())
    {
        qCritical() << "Could not read file with the groups list!";
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Не удалось прочитать файл!"));

        if(!userGroups_ || userGroups_->isEmpty())
        {
            this->reject();
        }

        ui->radioBtnList->toggle();
        return;
    }

    updateItems();
}

void AddTaskWindow::updateItems()
{
    if(!currentList_)
    {
        return;
    }

    this->groupsModel_->clear();
    std::for_each(std::cbegin(*currentList_), std::cend(*currentList_), [this](const auto& group)
    {
        QStandardItem* tmp = new QStandardItem(group["name"].toString());
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

std::shared_ptr<QJsonArray> AddTaskWindow::getGroupsFromJson(const QJsonDocument& document) const
{
    QJsonArray array = ((document["response"])["items"]).toArray();

    if(array.empty())
    {
        throw std::runtime_error{"groups list is empty!"};
    }

    return std::make_shared<QJsonArray>(std::move(array));
}

std::shared_ptr<QJsonArray> AddTaskWindow::readGroupsFromFile(const QString& filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return nullptr;
    }

    auto groups = std::make_shared<QJsonArray>(
                (QJsonDocument::fromJson(file.readAll())).array());
    file.close();
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
        qWarning() << "Failed to open the image!";
        QMessageBox::warning(this, tr("Ошибка"), tr("Ошибка чтения файла!"));
        return;
    }

    imgPath_ = filename;

    ui->loadImageLabel->show();
    updateLoadImg();
}

void AddTaskWindow::updateLoadImg()
{
    QSize sz = ui->loadImageLabel->size();
    ui->loadImageLabel->setPixmap(
                loadImg_.scaled(sz.width(), sz.height(),
                                Qt::KeepAspectRatio));
}

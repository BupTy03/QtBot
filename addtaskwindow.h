#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>

#include <algorithm>

namespace Ui {
class AddTaskWindow;
}

struct Group
{
    Group(QString id, QString name)
        : id(std::move(id)), name(std::move(name)){}
    QString id;
    QString name;
};

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent = nullptr);
    ~AddTaskWindow();

    int getInterval() const;
    int getPeriod() const;
    std::vector<int> getGroupsIndexes() const;
    QStringList getGroupsIds() const;
    QStringList getGroupsNames() const;
    QString getMessage() const;

private slots:
    void on_radioBtnList_toggled(bool checked);

    void on_radioBtnFile_toggled(bool checked);

private:
    void updateItems();
    std::vector<Group> getGroupsFromJson(const QJsonDocument& document) const;
    std::vector<Group> readGroupsFromFile(const QString& filename);

private:
    Ui::AddTaskWindow *ui;
    QStandardItemModel* groupsModel_;
    const std::vector<Group> userGroups_;
    std::vector<Group> currentList_;
};

#endif // ADDTASKWINDOW_H

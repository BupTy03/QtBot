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

QVector<std::shared_ptr<Group>> get_groups_from_json(const QJsonDocument& document);


class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    //explicit AddTaskWindow(const QStringList& groups,  QWidget* parent = nullptr);
    explicit AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent = nullptr);
    ~AddTaskWindow();

    int getInterval() const;
    int getPeriod() const;
    QVector<int> getGroupsIndexes() const;
    QStringList getGroupsIds() const;
    QStringList getGroupsNames() const;
    QString getMessage() const;

private slots:
    void on_radioBtnList_toggled(bool checked);

    void on_radioBtnFile_toggled(bool checked);

private:
    void updateItems();

private:
    Ui::AddTaskWindow *ui;
    QStandardItemModel* groupsModel_;
    QVector<std::shared_ptr<Group>> userGroups_;
    QVector<std::shared_ptr<Group>> currentList_;
};

#endif // ADDTASKWINDOW_H

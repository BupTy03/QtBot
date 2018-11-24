#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <algorithm>

namespace Ui {
class AddTaskWindow;
}

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(const QStringList& groups,  QWidget* parent = nullptr);
    ~AddTaskWindow();

    int getInterval();
    int getPeriod();
    QVector<int> getGroupsIndexes() const;
    QStringList getGroups() const;
    QString getMessage() const;

private slots:
    void on_radioBtnList_toggled(bool checked);

    void on_radioBtnFile_toggled(bool checked);

private:
    Ui::AddTaskWindow *ui;
    QStandardItemModel* groupsModel_;
    QStringList userGroups_;
    QStringList currentList_;
};

#endif // ADDTASKWINDOW_H

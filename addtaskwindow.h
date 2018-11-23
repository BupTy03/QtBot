#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class AddTaskWindow;
}

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(const QVector<QPair<QString, QString>>& groups,  QWidget* parent = nullptr);
    ~AddTaskWindow();

    int getInterval();
    int getPeriod();
    QVector<int> getGroupsIndexes();
    QString getMessage() const;

private:
    Ui::AddTaskWindow *ui;
    QStandardItemModel* groupsModel_;
};

#endif // ADDTASKWINDOW_H

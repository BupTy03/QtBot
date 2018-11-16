#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include "groupschoicemodel.h"

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
    explicit AddTaskWindow(const QStringList& groups,  QWidget* parent = nullptr);
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

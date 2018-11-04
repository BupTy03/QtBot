#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include "choicelistmodel.h"

#include <QDialog>

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
    ChoiceListModel* groupsModel_;
};

#endif // ADDTASKWINDOW_H

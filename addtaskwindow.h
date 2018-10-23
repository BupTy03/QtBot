#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include "groupschoicemodel.h"

#include <QDialog>

namespace Ui {
class AddTaskWindow;
}

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(const QVector<QPair<QString, QString>>& groups,  QWidget *parent = nullptr);
    ~AddTaskWindow();

    int getInterval();
    int getPeriod();
    const QSet<int>& getGroupsIndexesSet();
    QString getMessage();

private:
    Ui::AddTaskWindow *ui{nullptr};
    GroupsChoiceModel* groupsModel{nullptr};
};

#endif // ADDTASKWINDOW_H

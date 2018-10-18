#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>

namespace Ui {
class AddTaskWindow;
}

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(QWidget *parent = 0);
    ~AddTaskWindow();

    int getInterval();
    int getPeriod();
    QString getMessage();

private:
    Ui::AddTaskWindow *ui;
};

#endif // ADDTASKWINDOW_H

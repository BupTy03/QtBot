#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include "task.h"

#include "groupschoicemodel.h"
#include "myvk.h"

#include <QtWidgets>
#include <QScopedPointer>

class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(const Task& tsk, const MyVK* _vk, QWidget *parent = nullptr);

signals:
    void signalStart();
    void signalStop();
    void messageChanged(const QString&);

private slots:
    void onStartBtnClick();
    void onStopBtnClick();

private:
    GroupsChoiceModel* grChModel{nullptr};
    QListView* groupsView{nullptr};
    QTextEdit* messageEdit{nullptr};
    QSpinBox* intervalSB{nullptr};
    QSpinBox* periodSB{nullptr};
    QPushButton* startBtn{nullptr};
    QPushButton* stopBtn{nullptr};
};

#endif // TASKWIDGET_H

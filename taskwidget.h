#pragma once
#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include "task.h"
#include "choicelistmodel.h"

#include <QtWidgets>
#include <QScopedPointer>

class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(Task* task, const QStringList& groups_list, QWidget *parent = nullptr);

signals:
    void signalStart();
    void signalStop();
    void signalRemove();

private slots:
    void onStartBtnClick();
    void onStopBtnClick();
    void onRemoveBtnClick();

private:
    ChoiceListModel* grChModel_;
    QListView* groupsView_;
    QTextEdit* messageEdit_;
    QSpinBox* intervalSB_;
    QSpinBox* periodSB_;
    QPushButton* startBtn_;
    QPushButton* stopBtn_;
    QPushButton* removeBtn_;
};

#endif // TASKWIDGET_H

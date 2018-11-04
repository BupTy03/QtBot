#pragma once
#ifndef TASK_H
#define TASK_H

#include <QObject>

#include "vkauth.h"
#include "simpletimer.h"
#include "queries_to_vk.h"

#include <chrono>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(const QString& access_token,
                  const QStringList& groups,
                  const QString& message,
                  int interval,
                  int period,
                  QObject* parent = nullptr);

    explicit Task(QString&& access_token,
                  QStringList&& groups,
                  QString&& message,
                  int interval,
                  int period,
                  QObject* parent = nullptr);

    QString getMessage() const;
    int getInterval() const;
    int getPeriod() const;

    bool isRemoved() const;

public slots:
    void run();
    void start();
    void stop();
    void removeTask();

private:
    QStringList groupsIds_;
    QString accessToken_;
    QString message_;
    bool active_;
    int interval_;
    int period_;
    bool removed_;
    SimpleTimer<std::chrono::milliseconds> timer_;
};

#endif // TASK_H

#pragma once
#ifndef TASK_H
#define TASK_H

#include "vkauth.h"
#include "queries_to_vk.h"

#include <QObject>

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

    virtual void timerEvent(QTimerEvent* /*event*/) override;

    QString getMessage() const;
    int getInterval() const;
    int getPeriod() const;

public slots:
    void start();
    void stop();

private:
    QStringList groupsIds_;
    QString accessToken_;
    QString message_;
    bool active_;
    int interval_;
    int period_;
};

#endif // TASK_H

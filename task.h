#pragma once
#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimerEvent>

#include "vkauth.h"
#include "queries_to_vk.h"

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

    virtual void timerEvent(QTimerEvent* /*event*/) override;

    const QStringList& groupsIds() const;
    QString getMessage() const;
    int getInterval() const;
    int getPeriod() const;

public slots:
    void start();
    void stop();

private:
    bool active_{};
    int interval_{};
    int period_{};
    QStringList groupsIds_;
    QString accessToken_;
    QString message_;
};

#endif // TASK_H

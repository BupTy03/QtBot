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

    virtual void timerEvent(QTimerEvent* /*event*/) override;

    const QStringList& groupsIds() const;
    const QString& getMessage() const;
    int getInterval() const;
    int getPeriod() const;

    void setImgPath(const QString& img_path);

public slots:
    void start();
    void stop();
    void go();

private:
    QJsonDocument loadImage(const QString& group_id) const;
    QJsonDocument saveImage(const QString& group_id, const QJsonDocument& load_ans) const;
    void postToWall(const QString& group_id) const;

private:
    bool active_{};
    int interval_{};
    int period_{};
    QString accessToken_;
    QString message_;
    QString imgPath_;
    QStringList groupsIds_;
};

#endif // TASK_H

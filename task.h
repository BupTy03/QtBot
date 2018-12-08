#pragma once
#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimerEvent>

#include <memory>
#include <tuple>

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

    bool attachPhoto(const QString& img_path);

public slots:
    void start();
    void stop();
    void go();

private:
    std::tuple<QString, QString, QString> uploadPhoto(const QString& group_id) const;
    QString savePhoto(const QString& group_id,
                   const std::tuple<QString, QString, QString>& photoDetails) const;
    void postToWall(const QString& group_id) const;

private:
    bool active_{true};
    int interval_{};
    int period_{};
    QString accessToken_;
    QString message_;
    std::unique_ptr<QByteArray> photoAttachment_;
    QStringList groupsIds_;
};

#endif // TASK_H

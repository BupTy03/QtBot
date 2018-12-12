#pragma once
#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QTimerEvent>

#include <memory>
#include <vector>
#include <tuple>

#include "vkauth.h"
#include "queries_to_vk.h"

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(const QString& access_token,
                  const std::vector<std::pair<QString, QString>>& groups,
                  const QString& message,
                  int interval,
                  int period,
                  QObject* parent = nullptr);

    virtual void timerEvent(QTimerEvent* event) override;
    const std::vector<std::pair<QString, QString>>& getGroups() const { return groups_; }
    const QString& getMessage() const { return message_; }
    int getInterval() const { return interval_; }
    int getPeriod() const { return period_; }
    bool attachPhoto(const QString& img_path);

public slots:
    void start() { active_ = true; }
    void stop() { active_ = false; }
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
    QString photoPath_;
    std::vector<std::pair<QString, QString>> groups_;
};

#endif // TASK_H

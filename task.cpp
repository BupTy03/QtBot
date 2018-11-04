#include "task.h"

Task::Task(const QString& access_token,
           const QStringList& groups_ids,
           const QString& message,
           int interval,
           int period,
           QObject* parent)
    : QObject(parent),
      groupsIds_(groups_ids),
      accessToken_(access_token),
      message_(message),
      active_(true),
      interval_(interval),
      period_(period),
      removed_(false)
{
    timer_.start();
    timer_.set_interval(std::chrono::milliseconds(period*1000));
}

Task::Task(QString&& access_token, QStringList&& groups_ids, QString&& message, int interval, int period, QObject* parent)
    : QObject(parent),
      groupsIds_(std::move(groups_ids)),
      accessToken_(std::move(access_token)),
      message_(std::move(message)),
      active_(true),
      interval_(interval),
      period_(period),
      removed_(false)
{
    timer_.start();
    timer_.set_interval(std::chrono::milliseconds(period*1000));
}

QString Task::getMessage() const
{
    return message_;
}

int Task::getInterval() const
{
    return interval_;
}

int Task::getPeriod() const
{
    return period_;
}

bool Task::isRemoved() const
{
    return removed_;
}

void Task::run()
{
    if(removed_ || !active_ || !timer_.is_time_out())
    {
        return;
    }
    for(const auto& groupId : groupsIds_)
    {
        vk_query::messages_send_to_group(accessToken_, groupId, message_);
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_));
    }
    timer_.start();
}

void Task::start()
{
    active_ = true;
}

void Task::stop()
{
    active_ = false;
}

void Task::removeTask()
{
    removed_ = true;
}

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
      period_(period)
{
    startTimer(std::chrono::seconds(period));
}

void Task::timerEvent(QTimerEvent* event)
{
    if(!active_)
    {
        return;
    }

    for(const auto& groupId : groupsIds_)
    {
        vk_query::messages_send_to_group(accessToken_, groupId, message_);
        (this->thread())->msleep(static_cast<unsigned int>(interval_));
    }
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

void Task::start()
{
    active_ = true;
}

void Task::stop()
{
    active_ = false;
}

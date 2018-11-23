#include "task.h"

Task::Task(const QString& access_token,
           const QStringList& groups,
           const QString& message,
           int interval,
           int period,
           QObject* parent)
    : QObject(parent),
      groupsIds_(groups),
      accessToken_(access_token),
      message_(message),
      active_(true),
      interval_(interval),
      period_(period)
{
    startTimer(period_*1000);
}

Task::Task(QString&& access_token, QStringList&& groups, QString&& message, int interval, int period, QObject *parent)
    : QObject(parent),
      groupsIds_(std::move(groups)),
      accessToken_(std::move(access_token)),
      message_(std::move(message)),
      active_(true),
      interval_(interval),
      period_(period)
{
    startTimer(period_*1000);
}

void Task::timerEvent(QTimerEvent* /*event*/)
{
    if(!active_)
    {
        return;
    }

    for(const auto& groupId : groupsIds_)
    {
        vk_query::messages_send_to_group(accessToken_, groupId, message_);
        (this->thread())->msleep(static_cast<unsigned long>(interval_));
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

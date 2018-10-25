#include "task.h"

Task::Task(const QLinkedList<int>& indxs, const QString& text, int intrv, int per, QObject* parent)
    : QObject(parent), gr_indxes(indxs), text_of_msg(text), interval(intrv), period(per)
{
    start();
}

Task::Task(const Task& t)
{
    this->gr_indxes = t.gr_indxes;
    this->text_of_msg = t.text_of_msg;
    this->interval = t.interval;
    this->period = t.period;
    this->timer.setInterval(t.interval);
    this->active = t.active;

    start();
}

Task& Task::operator=(const Task& t)
{
    this->gr_indxes = t.gr_indxes;
    this->text_of_msg = t.text_of_msg;
    this->interval = t.interval;
    this->period = t.period;
    this->timer.setInterval(t.interval);
    this->active = t.active;

    start();
    return *this;
}

Task::Task(Task&& t)
{
    std::swap(this->gr_indxes, t.gr_indxes);
    std::swap(this->text_of_msg, t.text_of_msg);
    std::swap(this->interval, t.interval);
    std::swap(this->period, t.period);
    t.timer.stop();
    this->timer.setInterval(t.interval);
    std::swap(this->active, t.active);

    start();
}

Task& Task::operator=(Task&& t)
{
    std::swap(this->gr_indxes, t.gr_indxes);
    std::swap(this->text_of_msg, t.text_of_msg);
    std::swap(this->interval, t.interval);
    std::swap(this->period, t.period);
    t.timer.stop();
    this->timer.setInterval(t.interval);
    std::swap(this->active, t.active);

    start();
    return *this;
}

void Task::changeGroups(int id)
{
    if(gr_indxes.contains(id))
    {
        gr_indxes.removeAll(id);
    }
    else
    {
        gr_indxes.append(id);
    }

    qDebug() << *this << "\n";
}

#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QTimer>
#include <QDebug>
#include <QLinkedList>
#include <QDataStream>

class Task : public QObject
{
    Q_OBJECT
public:
    Task(){}
    explicit Task(const QLinkedList<int>& indxs, const QString& text, int intrv, int per, QObject* parent = nullptr);

    Task(const Task& t);
    Task& operator=(const Task& t);

    Task(Task&& t);
    Task& operator=(Task&& t);

public slots:
    void changeGroups(int id);
    void setMessage(const QString& text){ text_of_msg = text; qDebug() << "Changed message: " << text_of_msg; }
    void setGroups(const QLinkedList<int>& gr){ gr_indxes = gr; qDebug() << "Changed groups: " << text_of_msg; }
    void setInterval(int _int){ interval = _int; qDebug() << "Changed interval: " << interval; }
    void setPeriod(int _per){ period = _per; qDebug() << "Changed period: " << period; }

public:
    inline const QString& getMessage() const { return text_of_msg; }
    inline const QLinkedList<int>& getGroups() const { return gr_indxes; }
    inline int getInterval() const { return interval; }
    inline int getPeriod() const { return period; }

    inline friend bool operator < (const Task& tsk1, const Task& tsk2)
    {
        return tsk1.period < tsk2.period;
    }
    inline friend bool operator > (const Task& tsk1, const Task& tsk2)
    {
        return tsk1.period > tsk2.period;
    }

    inline friend bool operator == (const Task& tsk1, const Task& tsk2)
    {
        return &tsk1 == &tsk2;
    }
    inline friend bool operator != (const Task& tsk1, const Task& tsk2)
    {
        return &tsk1 != &tsk2;
    }

    inline friend bool operator <= (const Task& tsk1, const Task& tsk2)
    {
        return tsk1 < tsk2 || tsk1 == tsk2;
    }
    inline friend bool operator >= (const Task& tsk1, const Task& tsk2)
    {
        return tsk1 > tsk2 || tsk1 == tsk2;
    }

    inline bool isActive() noexcept { return active; }

    void start()
    {
        active = true;
        timer.start(period*1000);
    }
    void stop()
    {
        active = false;
        timer.stop();
    }

    friend QDebug operator<<(QDebug stream, const Task& t)
    {
        stream << "Indexes: [ ";
        for(int i : t.gr_indxes)
        {
            stream << i << " ";
        }
        stream << "]";
        stream <<
                  "\nText of the message: " << t.text_of_msg <<
                  "\nInterval: " << t.interval << "ms." <<
                  "\nPeriod: " << t.period << "ms." <<
                  "\nRemaining time: " << (t.timer).remainingTime() <<
                  "\nActive: " << (t.active ? "yes" : "no");

        return stream;
    }

    inline bool isTimeOut(){ return timer.remainingTime() == 0; }

private:
    QLinkedList<int> gr_indxes;
    QString text_of_msg;
    int interval;
    int period;
    QTimer timer;
    bool active{false};
};

#endif // TASK_H

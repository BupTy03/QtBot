#pragma once
#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include <type_traits>
#include <chrono>

template<typename T>
struct is_chrono_duration
{
    static const bool value =
        std::is_same<T, std::chrono::hours>::value			||
        std::is_same<T, std::chrono::minutes>::value		||
        std::is_same<T, std::chrono::seconds>::value		||
        std::is_same<T, std::chrono::milliseconds>::value	||
        std::is_same<T, std::chrono::microseconds>::value	||
        std::is_same<T, std::chrono::nanoseconds>::value;
};

template<typename T, typename = typename std::enable_if<is_chrono_duration<T>::value>::type>
class SimpleTimer
{
private:
    std::chrono::steady_clock::time_point begin_time_;
    std::chrono::steady_clock::time_point end_time_;
    T interval_;
    bool interval_was_set_;
    bool stopped_;

public:
    explicit SimpleTimer(bool stopped = true) : interval_was_set_(false), stopped_(stopped)
    {
        if (!stopped)
        {
            start();
        }
    }

    SimpleTimer(const SimpleTimer&) = default;
    SimpleTimer& operator=(const SimpleTimer&) = default;

    SimpleTimer(SimpleTimer&&) = default;
    SimpleTimer& operator=(SimpleTimer&&) = default;

    void set_interval(const T& interval)
    {
        interval_ = interval;
        interval_was_set_ = true;
    }

    bool is_time_out()
    {
        if (!interval_was_set_)
        {
            return false;
        }
        return elapsed_time() >= interval_;
    }

    T elapsed_time()
    {
        if (!stopped_)
        {
            end_time_ = std::chrono::steady_clock::now();
        }
        return std::chrono::duration_cast<T>(end_time_ - begin_time_);
    }

    void start()
    {
        stopped_ = false;
        begin_time_ = std::chrono::steady_clock::now();
    }

    void stop()
    {
        end_time_ = std::chrono::steady_clock::now();
        stopped_ = true;
    }
};

#endif // SIMPLETIMER_H

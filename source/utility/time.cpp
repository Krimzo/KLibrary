#include "utility/time.h"

#include "window/system_handler.h"


/* Time */
int64_t kl::time::now()
{
    LARGE_INTEGER time = {};
    QueryPerformanceCounter(&time);
    return time.QuadPart;
}

int64_t kl::time::cpu_frequency()
{
    LARGE_INTEGER frequency = {};
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

float kl::time::calculate(const int64_t start, const int64_t end)
{
    static const float rec_frequency = 1.0f / static_cast<float>(cpu_frequency());
    return static_cast<float>(end - start) * rec_frequency;
}

float kl::time::get_interval()
{
    static int64_t start_time = now();
    
    const int64_t end_time = now();
    const float elapsed_time = calculate(start_time, end_time);
    
    start_time = end_time;
    return elapsed_time;
}

void kl::time::wait(const float seconds)
{
    const int64_t start_time = now();
    while (calculate(start_time, now()) < seconds);
}

bool kl::time::sleep(const float seconds)
{
    const HANDLE timer = CreateWaitableTimerA(nullptr, true, nullptr);
    if (!timer) {
        return false;
    }

    static const int64_t frequency = cpu_frequency();
    const int64_t to_sleep = -static_cast<int64_t>(seconds * static_cast<float>(frequency));
    if (!SetWaitableTimer(timer, reinterpret_cast<const LARGE_INTEGER*>(&to_sleep), 0, nullptr, nullptr, false)) {
        CloseHandle(timer);
        return false;
    }

    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return true;
}

/* Date */
std::ostream& kl::operator<<(std::ostream& stream, const date& obj)
{
    stream << obj.year <<
        '.' << obj.month <<
        '.' << obj.day <<
        ' ' << obj.hour <<
        ':' << obj.minute <<
        ':' << obj.second;
    return stream;
}

/* Timer */
kl::timer::timer()
{
    const int64_t now = time::now();
    interval_start_ = now;
    interval_end_ = now;
    elapsed_start_ = now;
}

void kl::timer::reset()
{
    update_interval();
    update_elapsed();
}

void kl::timer::update_interval()
{
    interval_start_ = interval_end_;
    interval_end_ = time::now();
}

float kl::timer::get_interval() const
{
    return time::calculate(interval_start_, interval_end_);
}

void kl::timer::update_elapsed()
{
    elapsed_start_ = time::now();
}

float kl::timer::get_elapsed() const
{
    return time::calculate(elapsed_start_, time::now());
}

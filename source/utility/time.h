#pragma once

#include "math/math.h"


namespace kl::time {
    int64_t now();
    int64_t cpu_frequency();
    float calculate(int64_t start, int64_t end);

    float get_interval();
    void wait(float seconds);
    bool sleep(float seconds);
}

namespace kl {
    struct date
    {
        int year, month, day, hour, minute, second;

        date()
        {
            const int64_t time = std::time(nullptr);
            std::tm now = {};
            localtime_s(&now, &time);

            year = now.tm_year + 1900;
            month = now.tm_mon + 1;
            day = now.tm_mday;
            hour = now.tm_hour;
            minute = now.tm_min;
            second = now.tm_sec;
        }
    };

    std::ostream& operator<<(std::ostream& stream, const date& obj);
}

namespace kl {
    class timer
    {
        int64_t interval_start_;
        int64_t interval_end_;
        int64_t elapsed_start_;

    public:
        timer();

        void reset();

        void update_interval();
        [[nodiscard]] float get_interval() const;

        void update_elapsed();
        [[nodiscard]] float get_elapsed() const;
    };
}

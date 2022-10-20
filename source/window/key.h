#pragma once

#include <cstdint>
#include <functional>


namespace kl {
    class key
    {
        bool state_ = false;
        int64_t type_ = 0;

    public:
        std::function<void()> on_press = []
        {};
        std::function<void()> on_down = []
        {};
        std::function<void()> on_release = []
        {};

        explicit key(int64_t type);
        key(const key&) = delete;
        key(const key&&) = delete;
        ~key();

        void operator=(const key&) = delete;
        void operator=(const key&&) = delete;

        [[nodiscard]] bool state() const;

        void update_value(int64_t type, bool new_state);

        void process() const;
    };
}

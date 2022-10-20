#pragma once

#include "math/math.h"


namespace kl {
    void get();

    bool warning(bool occured, const std::string& message, bool wait = false);
    void assert(bool occured, const std::string& message, bool wait = true);
}

namespace kl::console {
    void set_enabled(bool state);
    void clear();

    void set_cursor_enabled(bool state);
    void move_cursor(const uint2& position);

    uint32_t width();
    void set_width(uint32_t width);

    uint32_t height();
    void set_height(uint32_t height);

    uint2 size();
    void set_size(const uint2& size);

    void set_title(const std::string& text);
    void set_font(const uint2& size, const std::string& font_name = "Consolas");

    char get_input();
    void wait(char to_wait_for, bool echo = false);
    char wait_for_any(bool echo = false);

    void dump(const std::string& data, const uint2& location = {});

    void progress_bar(const std::string& message, uint32_t output_y, float percentage);
}

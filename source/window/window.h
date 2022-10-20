#pragma once

#include "window/system_handler.h"
#include "window/keyboard.h"
#include "window/mouse.h"
#include "graphics/image.h"

#include <string>


namespace kl::screen {
    inline const uint2 size = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
}

namespace kl {
    class window
    {
        HINSTANCE instance_ = nullptr;
        std::string name_ = {};
        HWND window_ = nullptr;
        HDC device_context_ = nullptr;

        bool in_fullscreen_ = false;
        bool resizeable_ = true;
        DWORD window_style_ = NULL;
        WINDOWPLACEMENT placement_ = {};

        LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) const;
        void handle_message(const MSG& message);

    public:
        keyboard keyboard = {};
        mouse mouse = {};

        std::function<void(uint2)> on_resize = [](uint2)
        {};

        window(const uint2& size, const std::string& name);
        window(const window&) = delete;
        window(const window&&) = delete;
        ~window();
        
        void operator=(const window&) = delete;
        void operator=(const window&&) = delete;

        [[nodiscard]] HWND get_window() const;

        bool process(bool wait = true);

        [[nodiscard]] bool is_open() const;
        void close() const;

        [[nodiscard]] bool is_resizeable() const;
        void set_resizeable(bool enabled);

        void maximize() const;
        void minimize() const;

        [[nodiscard]] bool is_in_fullscreen() const;
        void set_fullscreen(bool enable);

        [[nodiscard]] int2 position(bool client = false) const;
        void set_position(const int2& position) const;

        [[nodiscard]] uint32_t width() const;
        void set_width(uint32_t width) const;

        [[nodiscard]] uint32_t height() const;
        void set_height(uint32_t height) const;

        [[nodiscard]] uint2 size(bool client = true) const;
        void set_size(const uint2& size, bool client = true) const;

        [[nodiscard]] float get_aspect_ratio() const;
        [[nodiscard]] uint2 get_frame_center() const;

        void set_title(const std::string& data) const;
        [[nodiscard]] bool set_icon(const std::string& filepath) const;

        void draw_pixel_data(const color* data, const uint2& size, const int2& position = {}) const;
        void draw_image(const image& image, const int2& position = {}) const;

        void notify() const;
    };
}

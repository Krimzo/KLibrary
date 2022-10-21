#include "utility/console.h"

#include "window/system_handler.h"
#include "utility/strings.h"
#include "math/math.h"

#include <iostream>
#include <sstream>


static const HANDLE console_handle = []
{
    const HANDLE temp_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD console_mode = {};
    GetConsoleMode(temp_handle, &console_mode);
    SetConsoleMode(temp_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    return temp_handle;
}();

void kl::get()
{
    std::cin.get();
}

bool kl::warning(const bool occured, const std::string& message, const bool wait)
{
    if (occured) {
        print(colors::orange, "Warning: ", message);

        if (wait) {
            console::wait_for_any();
        }

        print<false>(colors::console);
    }
    return occured;
}

void kl::assert(const bool occured, const std::string& message, const bool wait)
{
    if (occured) {
        print(colors::red, "Error: ", message);

        if (wait) {
            console::wait_for_any();
        }

        print<false>(colors::console);
        static_assert(true);
    }
}

void kl::console::set_enabled(const bool state)
{
    static HWND console_window = GetConsoleWindow();
    if (state) {
        ShowWindow(console_window, SW_SHOW);

    }
    else {
        ShowWindow(console_window, SW_HIDE);
    }
}

void kl::console::clear()
{
    CONSOLE_SCREEN_BUFFER_INFO console_screen_info = {};
    GetConsoleScreenBufferInfo(console_handle, &console_screen_info);

    DWORD chars_written = {};
    FillConsoleOutputCharacterA(console_handle, ' ', console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {},
                                &chars_written);

    FillConsoleOutputAttribute(
        console_handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written
    );

    move_cursor({});
}

void kl::console::set_cursor_enabled(const bool state)
{
    CONSOLE_CURSOR_INFO cursor_info = {};
    GetConsoleCursorInfo(console_handle, &cursor_info);
    cursor_info.bVisible = state;
    SetConsoleCursorInfo(console_handle, &cursor_info);
}

void kl::console::move_cursor(const uint2& position)
{
    SetConsoleCursorPosition(console_handle, {static_cast<short>(position.x), static_cast<short>(position.y)});
}

uint32_t kl::console::width()
{
    return size().x;
}

void kl::console::set_width(uint32_t width)
{
    set_size({width, height()});
}

uint32_t kl::console::height()
{
    return size().y;
}

void kl::console::set_height(uint32_t height)
{
    set_size({width(), height});
}

void kl::console::set_title(const std::string& text)
{
    SetConsoleTitleA(text.c_str());
}

kl::uint2 kl::console::size()
{
    CONSOLE_SCREEN_BUFFER_INFO info = {};
    GetConsoleScreenBufferInfo(console_handle, &info);
    return {info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1};
}

void kl::console::set_size(const uint2& size)
{
    const SMALL_RECT rect = {0, 0, static_cast<short>(size.x - 1), static_cast<short>(size.y - 1)};
    SetConsoleWindowInfo(console_handle, true, &rect);
}

void kl::console::set_font(const uint2& size, const std::string& font_name)
{
    CONSOLE_FONT_INFOEX cfi = {};
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.X = static_cast<short>(size.x);
    cfi.dwFontSize.Y = static_cast<short>(size.y);
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, strings::to_w_string(font_name).c_str());
    SetCurrentConsoleFontEx(console_handle, false, &cfi);
}

char kl::console::get_input()
{
    char input = 0;
    while (_kbhit()) {
        input = static_cast<char>(_getch());
    }
    return input;
}

void kl::console::wait(const char to_wait_for, const bool echo)
{
    if (echo) {
        if (to_wait_for > 31 && to_wait_for < 127) {
            print("Press '", to_wait_for, "' to continue...");
        }
        else {
            print("Press '", static_cast<int>(to_wait_for), "' to continue...");
        }
    }

    char c;
    while ((c = static_cast<char>(_getch())) != to_wait_for) {
        print(static_cast<int>(c));
    }
}

char kl::console::wait_for_any(const bool echo)
{
    if (echo) {
        print("Press any key to continue...");
    }
    return static_cast<char>(_getch());
}

void kl::console::dump(const std::string& data, const uint2& location)
{
    static DWORD ignored = 0;
    WriteConsoleOutputCharacterA(console_handle, data.c_str(), static_cast<DWORD>(data.length()),
                                 {static_cast<short>(location.x), static_cast<short>(location.y)}, &ignored);
}

void kl::console::progress_bar(const std::string& message, uint32_t output_y, float percentage)
{
    percentage = math::minmax(percentage, 0, 1);

    const int bar_length = static_cast<int>(width()) - static_cast<int>(message.length()) - 12;
    const int finish_length = static_cast<int>(static_cast<float>(bar_length) * percentage);
    const int empty_length = bar_length - finish_length;

    std::stringstream stream = {};
    stream << "  " << message << " [";
    for (int i = 0; i < finish_length; i++) {
        stream << '#';
    }
    for (int i = 0; i < empty_length; i++) {
        stream << ' ';
    }
    move_cursor({0, output_y});

    printf("%s] %3d%% ", stream.str().c_str(), static_cast<int>(percentage * 100.0f));
}

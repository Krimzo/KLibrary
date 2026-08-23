#pragma once

#include "window/input/key.h"
#include "window/input/keyboard.h"
#include "window/input/mouse.h"
#include "window/hooks/keyboard_hook.h"
#include "window/hooks/mouse_hook.h"


namespace kl
{
inline const int2 SCREEN_SIZE = {
    GetSystemMetrics( SM_CXSCREEN ),
    GetSystemMetrics( SM_CYSCREEN ),
};
}

namespace kl
{
struct Window : NoCopy
{
    Keyboard keyboard;
    Mouse mouse;

    std::function<void( int2, std::vector<std::wstring> const& )> on_drag_drop = []( int2, std::vector<std::wstring> const& ) {};
    std::vector<std::function<void( int2 )>> on_resize;
    std::vector<std::function<void( int2 )>> on_move;

    Window( std::string_view const& name );
    ~Window();

    HWND ptr() const;
    bool process();

    bool active() const;
    void close() const;

    bool resizeable() const;
    void set_resizeable( bool enabled );

    bool fullscreened() const;
    void set_fullscreen( bool enabled );

    LONG style() const;
    void add_style( LONG style_val );
    void remove_style( LONG style_val );

    bool focused() const;

    void maximize() const;
    void minimize() const;
    void restore() const;

    bool maximized() const;
    bool minimized() const;
    bool restored() const;

    int2 position() const;
    void set_position( int2 position ) const;

    int width() const;
    void set_width( int width ) const;

    int height() const;
    void set_height( int height ) const;

    int2 size() const;
    void resize( int2 size ) const;

    float aspect_ratio() const;
    int2 frame_center() const;

    float dpi() const;
    float pixels_to_dips( float value ) const;
    float dips_to_pixels( float value ) const;

    void set_title( std::string_view const& data ) const;
    bool set_icon( std::string_view const& filepath ) const;

    void draw_pixel_data( RGB const* data, int2 size, int2 position = {} ) const;
    void draw_image( Image const& image, int2 position = {} ) const;

    void set_dark_mode( bool enabled ) const;
    void notify() const;

private:
    std::string m_name;
    HINSTANCE m_instance = nullptr;
    HWND m_window = nullptr;
    HDC m_device_context = nullptr;

    LRESULT CALLBACK window_procedure( HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param ) const;
    void handle_message( MSG const& message );
};
}

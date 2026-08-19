#pragma once

#include "math/math.h"


namespace kl
{
enum struct ImageType : int32_t
{
    BMP = 0,
    PNG,
    JPG,
};
}

namespace kl
{
struct Image
{
    Image() = default;
    Image( int2 size );
    Image( std::string_view const& filepath );
    Image( std::wstring_view const& filepath );

    RGB* ptr();
    RGB const* ptr() const;

    int pixel_count() const;
    uint64_t byte_size() const;

    RGB& operator[]( int index );
    RGB const& operator[]( int index ) const;

    RGB& operator[]( int2 coords );
    RGB const& operator[]( int2 coords ) const;

    bool in_bounds( int2 coords ) const;
    RGB sample( float2 uv ) const;

    int width() const;
    void set_width( int width );

    int height() const;
    void set_height( int height );

    int2 size() const;
    void resize( int2 new_size );
    void resize_scaled( int2 new_size );

    void fill( RGB color );

    Image flip_horizontal() const;
    Image flip_vertical() const;

    Image rectangle( int2 top_left, int2 bottom_right ) const;
    std::string as_ascii( int2 frame_size ) const;

    void draw_line( int2 from, int2 to, RGB color );
    void draw_triangle( int2 position_a, int2 position_b, int2 position_c, RGB color, bool fill = false );
    void draw_rectangle( int2 top_left, int2 bottom_right, RGB color, bool fill = false );
    void draw_circle( int2 center, float radius, RGB color, bool fill = false );
    void draw_circle( int2 center, int2 outer_position, RGB color, bool fill = false );
    void draw_image( int2 top_left, Image const& image, bool mix_alpha = true );

    bool load_from_memory( void const* data, uint64_t byte_size );
    bool load_from_buffer( std::string_view const& buffer );
    bool load_from_file( std::string_view const& filepath );
    bool load_from_file( std::wstring_view const& filepath );

    bool save_to_buffer( std::string& buffer, ImageType type ) const;
    bool save_to_file( std::string_view const& filepath, ImageType type ) const;
    bool save_to_file( std::wstring_view const& filepath, ImageType type ) const;

private:
    std::vector<RGB> m_pixels;
    int2 m_size;
};
}

namespace kl
{
Image take_screenshot();
}

namespace kl
{
struct ImageInit
{
    static const int _init;
};
}

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
    Image( Int2 size );
    Image( std::string_view const& filepath );
    Image( std::wstring_view const& filepath );

    RGB* ptr();
    RGB const* ptr() const;

    int pixel_count() const;
    uint64_t byte_size() const;

    RGB& operator[]( int index );
    RGB const& operator[]( int index ) const;

    RGB& operator[]( Int2 coords );
    RGB const& operator[]( Int2 coords ) const;

    bool in_bounds( Int2 coords ) const;
    RGB sample( Float2 uv ) const;

    int width() const;
    void set_width( int width );

    int height() const;
    void set_height( int height );

    Int2 size() const;
    void resize( Int2 new_size );
    void resize_scaled( Int2 new_size );

    void fill( RGB color );

    Image flip_horizontal() const;
    Image flip_vertical() const;

    Image rectangle( Int2 top_left, Int2 bottom_right ) const;
    std::string as_ascii( Int2 frame_size ) const;

    void draw_line( Int2 from, Int2 to, RGB color );
    void draw_triangle( Int2 position_a, Int2 position_b, Int2 position_c, RGB color, bool fill = false );
    void draw_rectangle( Int2 top_left, Int2 bottom_right, RGB color, bool fill = false );
    void draw_circle( Int2 center, float radius, RGB color, bool fill = false );
    void draw_circle( Int2 center, Int2 outer_position, RGB color, bool fill = false );
    void draw_image( Int2 top_left, Image const& image, bool mix_alpha = true );

    bool load_from_memory( void const* data, uint64_t byte_size );
    bool load_from_buffer( std::string_view const& buffer );
    bool load_from_file( std::string_view const& filepath );
    bool load_from_file( std::wstring_view const& filepath );

    bool save_to_buffer( std::string& buffer, ImageType type ) const;
    bool save_to_file( std::string_view const& filepath, ImageType type ) const;
    bool save_to_file( std::wstring_view const& filepath, ImageType type ) const;

private:
    std::vector<RGB> m_pixels;
    Int2 m_size;
};
}

namespace kl
{
Image take_screenshot();
}

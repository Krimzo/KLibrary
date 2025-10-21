#include "klibrary.h"


static int _image_init = []
    {
        ULONG_PTR token = NULL;
        Gdiplus::GdiplusStartupInput startup_input = {};
        kl::assert( !GdiplusStartup( &token, &startup_input, nullptr ), "Failed to init GDIPlus" );
        return 0;
    }( );

kl::Image::Image( Int2 size )
    : m_size( size )
{
    m_pixels.resize( (size_t) size.x * size.y );
}

kl::Image::Image( std::string_view const& filepath )
{
    load_from_file( filepath );
}

kl::Image::Image( std::wstring_view const& filepath )
{
    load_from_file( filepath );
}

kl::RGB* kl::Image::ptr()
{
    return m_pixels.data();
}

kl::RGB const* kl::Image::ptr() const
{
    return m_pixels.data();
}

int kl::Image::pixel_count() const
{
    return m_size.x * m_size.y;
}

uint64_t kl::Image::byte_size() const
{
    return pixel_count() * sizeof( RGB );
}

kl::RGB& kl::Image::operator[]( int index )
{
    return m_pixels[index];
}

kl::RGB const& kl::Image::operator[]( int index ) const
{
    return m_pixels[index];
}

kl::RGB& kl::Image::operator[]( Int2 coords )
{
    return m_pixels[coords.to_index( m_size.x )];
}

kl::RGB const& kl::Image::operator[]( Int2 coords ) const
{
    return m_pixels[coords.to_index( m_size.x )];
}

bool kl::Image::in_bounds( Int2 coords ) const
{
    return coords.in_bounds( m_size );
}

kl::RGB kl::Image::sample( Float2 uv ) const
{
    Int2 coords = {
        int( uv.x * m_size.x ),
        int( uv.y * m_size.y ),
    };
    if ( in_bounds( coords ) )
    {
        return ( *this )[coords];
    }
    return {};
}

int kl::Image::width() const
{
    return m_size.x;
}

void kl::Image::set_width( int width )
{
    resize( { width, m_size.y } );
}

int kl::Image::height() const
{
    return m_size.y;
}

void kl::Image::set_height( int height )
{
    resize( { m_size.x, height } );
}

kl::Int2 kl::Image::size() const
{
    return m_size;
}

void kl::Image::resize( Int2 new_size )
{
    if ( new_size == m_size ) { return; }

    int min_x = min( new_size.x, m_size.x );
    int min_y = min( new_size.y, m_size.y );

    Image result{ new_size };
    for ( Int2 position; position.y < min_y; position.y++ )
    {
        for ( position.x = 0; position.x < min_x; position.x++ )
            result[position] = ( *this )[position];
    }
    *this = result;
}

void kl::Image::resize_scaled( Int2 new_size )
{
    if ( new_size == m_size ) { return; }

    float ratio_x = (float) m_size.x / new_size.x;
    float ratio_y = (float) m_size.y / new_size.y;

    Image result = { new_size };
    for ( Int2 position; position.y < new_size.y; position.y++ )
    {
        for ( position.x = 0; position.x < new_size.x; position.x++ )
        {
            Int2 read_position = { int( position.x * ratio_x ), int( position.y * ratio_y ) };
            if ( in_bounds( read_position ) )
                result[position] = ( *this )[read_position];
        }
    }
    *this = result;
}

void kl::Image::fill( RGB color )
{
    for ( auto& pixel : m_pixels )
        pixel = color;
}

kl::Image kl::Image::flip_horizontal() const
{
    Image result = { size() };
    for ( int y = 0; y < m_size.y; y++ )
    {
        for ( int x = 0; x < m_size.x; x++ )
            result[{ x, y }] = ( *this )[{ ( m_size.x - 1 - x ), y }];
    }
    return result;
}

kl::Image kl::Image::flip_vertical() const
{
    Image result = { size() };
    for ( int x = 0; x < m_size.x; x++ )
    {
        for ( int y = 0; y < m_size.y; y++ )
            result[{ x, y }] = ( *this )[{ x, ( m_size.y - 1 - y ) }];
    }
    return result;
}

kl::Image kl::Image::rectangle( Int2 top_left, Int2 bottom_right ) const
{
    if ( bottom_right.x < top_left.x )
    {
        std::swap( top_left.x, bottom_right.x );
    }
    if ( bottom_right.y < top_left.y )
    {
        std::swap( top_left.y, bottom_right.y );
    }
    Image result = { Int2( bottom_right.x - top_left.x, bottom_right.y - top_left.y ) };
    for ( Int2 position; position.y < result.height(); position.y++ )
    {
        for ( position.x = 0; position.x < result.width(); position.x++ )
        {
            Int2 read_position = top_left + position;
            if ( in_bounds( read_position ) )
                result[position] = ( *this )[read_position];
        }
    }
    return result;
}

std::string kl::Image::as_ascii( Int2 frame_size ) const
{
    Int2 increment = m_size / frame_size;
    std::stringstream frame;
    for ( Int2 position; position.y < frame_size.y; position.y++ )
    {
        for ( position.x = 0; position.x < frame_size.x; position.x++ )
        {
            Int2 read_position = { position.x * increment.x, position.y * increment.y };
            if ( in_bounds( read_position ) )
                frame << ( *this )[read_position].ascii();
        }
    }
    return frame.str();
}

void kl::Image::draw_line( Int2 from, Int2 to, RGB color )
{
    int length = max( abs( to.x - from.x ), abs( to.y - from.y ) );
    Float2 increment = { ( to.x - from.x ) / (float) length, ( to.y - from.y ) / (float) length };
    Float2 draw_point = from;
    for ( int i = 0; i <= length; i++ )
    {
        if ( in_bounds( draw_point ) )
            ( *this )[draw_point] = color;

        draw_point += increment;
    }
}

void kl::Image::draw_triangle( Int2 position_a, Int2 position_b, Int2 position_c, RGB color, bool fill )
{
    if ( !fill )
    {
        draw_line( position_a, position_b, color );
        draw_line( position_b, position_c, color );
        draw_line( position_c, position_a, color );
        return;
    }
    if ( position_a.y > position_b.y )
    {
        std::swap( position_a, position_b );
    }
    if ( position_a.y > position_c.y )
    {
        std::swap( position_a, position_c );
    }
    if ( position_b.y > position_c.y )
    {
        std::swap( position_b, position_c );
    }
    for ( int y = position_a.y; y < position_c.y; y++ )
    {
        Float2 flt_pos_a = position_a;
        Float2 flt_pos_b = position_b;
        Float2 flt_pos_c = position_b;
        draw_line(
            { (int) line_x( ( y < position_b.y ) ? flt_pos_a : flt_pos_c, flt_pos_b, (float) y ), y },
            { (int) line_x( flt_pos_a, flt_pos_c, (float) y ), y },
            color
        );
    }
}

void kl::Image::draw_rectangle( Int2 top_left, Int2 bottom_right, RGB color, bool fill )
{
    if ( !fill )
    {
        draw_line( top_left, { top_left.x, bottom_right.y }, color );
        draw_line( top_left, { bottom_right.x, top_left.y }, color );
        draw_line( bottom_right, { top_left.x, bottom_right.y }, color );
        draw_line( bottom_right, { bottom_right.x, top_left.y }, color );
        return;
    }
    if ( top_left.y > bottom_right.y )
    {
        std::swap( top_left, bottom_right );
    }
    for ( int y = top_left.y; y <= bottom_right.y; y++ )
    {
        draw_line( { top_left.x, y }, { bottom_right.x, y }, color );
    }
}

void kl::Image::draw_circle( Int2 center, float radius, RGB color, bool fill )
{
    Float2 f_center = center;
    if ( fill )
    {
        int start = (int) ( f_center.y - radius );
        int end = (int) ( f_center.y + radius );

        for ( int y = start; y <= end; y++ )
        {
            int x = int( f_center.x + sqrt( radius * radius - ( y - f_center.y ) * ( y - f_center.y ) ) );
            draw_line( { 2 * center.x - x, y }, { x, y }, color );
        }
        return;
    }
    int end = int( 2 * radius );
    for ( int i = 0; i < end; i++ )
    {
        int x1 = (int) ( f_center.x - radius + i );
        int y1 = (int) ( f_center.y + sqrt( radius * radius - ( x1 - f_center.x ) * ( x1 - f_center.x ) ) );

        if ( Int2 write_position = { x1, y1 }; in_bounds( write_position ) )
        {
            ( *this )[write_position] = color;
        }
        if ( Int2 write_position = { x1, 2 * center.y - y1 }; in_bounds( write_position ) )
        {
            ( *this )[write_position] = color;
        }

        int y2 = (int) ( f_center.y - radius + i );
        int x2 = (int) ( f_center.x + sqrt( radius * radius - ( y2 - f_center.y ) * ( y2 - f_center.y ) ) );

        if ( Int2 write_position = { x2, y2 }; in_bounds( write_position ) )
        {
            ( *this )[write_position] = color;
        }
        if ( Int2 write_position = { 2 * center.x - x2, y2 }; in_bounds( write_position ) )
        {
            ( *this )[write_position] = color;
        }
    }
}

void kl::Image::draw_circle( Int2 center, Int2 outer_position, RGB color, bool fill )
{
    draw_circle( center, Float2( outer_position - center ).length(), color, fill );
}

void kl::Image::draw_image( Int2 top_left, Image const& image, bool mix_alpha )
{
    for ( Int2 position; position.y < image.height(); position.y++ )
    {
        for ( position.x = 0; position.x < image.width(); position.x++ )
        {
            Int2 write_position = { top_left.x + position.x, top_left.y + position.y };
            if ( in_bounds( write_position ) )
            {
                RGB result_pixel = mix_alpha ? ( *this )[write_position].mix( image[position] ) : image[position];
                ( *this )[write_position] = result_pixel;
            }
        }
    }
}

bool kl::Image::load_from_memory( void const* data, uint64_t byte_size )
{
    ComRef<IStream> stream{ SHCreateMemStream( reinterpret_cast<BYTE const*>( data ), UINT( byte_size ) ) };
    Gdiplus::Bitmap loaded_bitmap( stream.get() );
    if ( !verify( !loaded_bitmap.GetLastStatus(), "Failed to decode image" ) )
        return false;

    Gdiplus::BitmapData bitmap_data{};
    loaded_bitmap.LockBits( nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmap_data );
    if ( !verify( bitmap_data.Scan0, "Failed to load image data" ) )
        return false;

    resize( { int( bitmap_data.Width ), int( bitmap_data.Height ) } );
    copy<RGB>( m_pixels.data(), bitmap_data.Scan0, m_pixels.size() );
    return true;
}

bool kl::Image::load_from_buffer( std::string_view const& buffer )
{
    return load_from_memory( buffer.data(), buffer.size() );
}

bool kl::Image::load_from_file( std::string_view const& filepath )
{
    return load_from_buffer( read_file_string( filepath ) );
}

bool kl::Image::load_from_file( std::wstring_view const& filepath )
{
    return load_from_buffer( read_file_string( filepath ) );
}

bool kl::Image::save_to_buffer( std::string& buffer, ImageType type ) const
{
    static constexpr CLSID bmp_encoder_clsid = {
        0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e },
    };
    static constexpr CLSID jpg_encoder_clsid = {
        0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e },
    };
    static constexpr CLSID png_encoder_clsid = {
        0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e },
    };

    CLSID const* format_to_use;
    if ( type == ImageType::BMP )
    {
        format_to_use = &bmp_encoder_clsid;
    }
    else if ( type == ImageType::PNG )
    {
        format_to_use = &png_encoder_clsid;
    }
    else if ( type == ImageType::JPG )
    {
        format_to_use = &jpg_encoder_clsid;
    }
    else
    {
        return false;
    }

    Gdiplus::Bitmap bitmap( m_size.x, m_size.y, PixelFormat32bppARGB );
    if ( !verify( !bitmap.GetLastStatus(), "Failed to create bitmap" ) )
        return false;

    Gdiplus::BitmapData bitmap_data{};
    bitmap.LockBits( nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmap_data );
    if ( !verify( bitmap_data.Scan0, "Failed to lock bitmap bits" ) )
        return false;

    copy<RGB>( bitmap_data.Scan0, m_pixels.data(), m_pixels.size() );
    bitmap.UnlockBits( &bitmap_data );

    ComRef<IStream> stream{ SHCreateMemStream( nullptr, 0 ) };
    bitmap.Save( stream.get(), format_to_use, nullptr );

    STATSTG stream_info{};
    stream->Stat( &stream_info, STATFLAG_NONAME );
    buffer.resize( stream_info.cbSize.QuadPart );

    stream->Seek( {}, STREAM_SEEK_SET, nullptr );
    stream->Read( buffer.data(), (ULONG) buffer.size(), nullptr );
    return true;
}

bool kl::Image::save_to_file( std::string_view const& filepath, ImageType type ) const
{
    std::string buffer;
    if ( save_to_buffer( buffer, type ) )
        return write_file_string( filepath, buffer );
    return false;
}

bool kl::Image::save_to_file( std::wstring_view const& filepath, ImageType type ) const
{
    std::string buffer;
    if ( save_to_buffer( buffer, type ) )
        return write_file_string( filepath, buffer );
    return false;
}

kl::Image kl::take_screenshot()
{
    HDC screen_dc = GetDC( nullptr );
    HDC memory_dc = CreateCompatibleDC( screen_dc );

    int width = GetDeviceCaps( screen_dc, HORZRES );
    int height = GetDeviceCaps( screen_dc, VERTRES );

    HBITMAP bitmap = CreateCompatibleBitmap( screen_dc, width, height );
    HBITMAP old_bitmap = (HBITMAP) SelectObject( memory_dc, bitmap );

    BitBlt( memory_dc, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY );
    bitmap = (HBITMAP) SelectObject( memory_dc, old_bitmap );

    Image result{ Int2{ width, height } };
    GetBitmapBits( bitmap, width * height * (LONG) sizeof( RGB ), result.ptr() );

    DeleteDC( memory_dc );
    DeleteDC( screen_dc );
    DeleteObject( old_bitmap );
    DeleteObject( bitmap );
    return result;
}

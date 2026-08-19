#include "klibrary.h"


kl::RasterText::RasterText( TextFormat const& format, std::string_view const& data, float4 const& color, float2 tl, float2 br, HAlign h_align, VAlign v_align )
    : format( format )
    , data( convert_string( data ) )
    , color( color )
    , box_top_left( tl )
    , box_bottom_right( br )
    , h_align( h_align )
    , v_align( v_align )
{}

kl::RasterText::RasterText( TextFormat const& format, std::wstring_view const& data, float4 const& color, float2 tl, float2 br, HAlign h_align, VAlign v_align )
    : format( format )
    , data( data )
    , color( color )
    , box_top_left( tl )
    , box_bottom_right( br )
    , h_align( h_align )
    , v_align( v_align )
{}

void kl::RasterText::draw( ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush ) const
{
    if ( !format )
        return;
    const float2 target_size = (float2 const&) target->GetSize();
    const float2 top_left_screen = from_ndc( box_top_left, target_size );
    const float2 bottom_right_screen = from_ndc( box_bottom_right, target_size );
    const D2D1_RECT_F layout_rect{
        .left = top_left_screen.x,
        .top = top_left_screen.y,
        .right = bottom_right_screen.x,
        .bottom = bottom_right_screen.y,
    };
    format->SetTextAlignment( h_align );
    format->SetParagraphAlignment( v_align );
    brush->SetColor( D2D1_COLOR_F{
        .r = color.x,
        .g = color.y,
        .b = color.z,
        .a = color.w,
        } );
    target->DrawTextW(
        data.data(),
        (UINT) data.size(),
        format.get(),
        layout_rect,
        brush.get() );
}

kl::RasterLine::RasterLine( float2 start, float2 end, float4 color, float stroke_width )
    : start( start )
    , end( end )
    , color( color )
    , stroke_width( stroke_width )
{}

void kl::RasterLine::draw( ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush ) const
{
    const float2 target_size = (float2 const&) target->GetSize();
    brush->SetColor( D2D1_COLOR_F{
        .r = color.x,
        .g = color.y,
        .b = color.z,
        .a = color.w,
        } );
    target->DrawLine(
        (D2D1_POINT_2F const&) from_ndc( start, target_size ),
        (D2D1_POINT_2F const&) from_ndc( end, target_size ),
        brush.get(),
        stroke_width );
}

kl::RasterRectangle::RasterRectangle( float2 const& top_left, float2 const& bottom_right, float4 const& color, bool fill, float2 const& corner_radius, float stroke_width )
    : top_left( top_left )
    , bottom_right( bottom_right )
    , color( color )
    , fill( fill )
    , corner_radius( corner_radius )
    , stroke_width( stroke_width )
{}

void kl::RasterRectangle::draw( ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush ) const
{
    const float2 target_size = (float2 const&) target->GetSize();
    const float2 tl_px = from_ndc( top_left, target_size );
    const float2 br_px = from_ndc( bottom_right, target_size );
    const D2D1_ROUNDED_RECT round_rect{
        .rect = {
            .left = tl_px.x,
            .top = tl_px.y,
            .right = br_px.x,
            .bottom = br_px.y,
    },
    .radiusX = corner_radius.x,
    .radiusY = corner_radius.y,
    };
    brush->SetColor( D2D1_COLOR_F{
        .r = color.x,
        .g = color.y,
        .b = color.z,
        .a = color.w,
        } );
    if ( fill )
        target->FillRoundedRectangle( round_rect, brush.get() );
    else
        target->DrawRoundedRectangle( round_rect, brush.get(), stroke_width );
}

kl::RasterEllipse::RasterEllipse( float2 const& center, float2 const& radius, float4 const& color, bool fill, float stroke_width )
    : center( center )
    , radius( radius )
    , color( color )
    , fill( fill )
    , stroke_width( stroke_width )
{}

void kl::RasterEllipse::draw( ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush ) const
{
    const float2 target_size = (float2 const&) target->GetSize();
    const float2 center_px = from_ndc( center, target_size );
    const float2 radius_px = radius * target_size * 0.5f;
    const D2D1_ELLIPSE elipse{
        .point = (D2D1_POINT_2F const&) center_px,
        .radiusX = radius_px.x,
        .radiusY = radius_px.y,
    };
    brush->SetColor( D2D1_COLOR_F{
        .r = color.x,
        .g = color.y,
        .b = color.z,
        .a = color.w,
        } );
    if ( fill )
        target->FillEllipse( elipse, brush.get() );
    else
        target->DrawEllipse( elipse, brush.get(), stroke_width );
}

kl::Raster::Raster()
{
    D2D1_FACTORY_OPTIONS options{};
    options.debugLevel = D2D1_DEBUG_LEVEL( kl::IS_DEBUG ? ( D2D1_DEBUG_LEVEL_WARNING | D2D1_DEBUG_LEVEL_ERROR ) : D2D1_DEBUG_LEVEL_NONE );
    D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof( ID2D1Factory ), &options, (void**) &m_d2d1_factory ) >> verify_result;
    DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), (IUnknown**) &m_write_factory ) >> verify_result;
}

kl::TextFormat kl::Raster::create_text_format(
    std::wstring_view const& font_family,
    DWRITE_FONT_WEIGHT font_weight,
    DWRITE_FONT_STYLE font_style,
    float font_size,
    std::wstring_view const& locale
) const
{
    if ( font_size <= 0.0f )
        return {};
    TextFormat format;
    m_write_factory->CreateTextFormat(
        font_family.data(), nullptr, font_weight, font_style, DWRITE_FONT_STRETCH_NORMAL,
        font_size, locale.data(), &format
    ) >> verify_result;
    return format;
}

void kl::Raster::draw_raster_batch( UINT target_index ) const
{
    auto const& target = m_d2d1_targets[target_index];
    ComRef<ID2D1SolidColorBrush> brush;
    target->CreateSolidColorBrush( {}, &brush );
    target->BeginDraw();
    for ( auto const& raster_item : raster_batch )
        raster_item->draw( target, brush );
    target->EndDraw();
}

void kl::Raster::draw_raster_direct( UINT target_index, RasterItem const& raster_item ) const
{
    auto const& target = m_d2d1_targets[target_index];
    ComRef<ID2D1SolidColorBrush> brush;
    target->CreateSolidColorBrush( {}, &brush );
    target->BeginDraw();
    raster_item.draw( target, brush );
    target->EndDraw();
}

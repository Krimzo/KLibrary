#include "klibrary.h"


kl::TextRaster::TextRaster()
{
    D2D1_FACTORY_OPTIONS options{};
    options.debugLevel = D2D1_DEBUG_LEVEL( kl::IS_DEBUG ? ( D2D1_DEBUG_LEVEL_WARNING | D2D1_DEBUG_LEVEL_ERROR ) : D2D1_DEBUG_LEVEL_NONE );
    D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof( ID2D1Factory ), &options, (void**) &m_d2d1_factory ) >> verify_result;
    DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), (IUnknown**) &m_write_factory ) >> verify_result;
}

kl::TextFormat kl::TextRaster::create_text_format(
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

void kl::TextRaster::draw_text_batch( UINT target_index ) const
{
    auto const& target = m_d2d1_targets[target_index];
    const Float2 target_size = (Float2 const&) target->GetSize();
    ComRef<ID2D1SolidColorBrush> brush;
    D2D1_RECT_F layout_rect{};

    target->BeginDraw();
    for ( auto const& text : text_batch )
    {
        if ( !text.format )
            continue;

        const Float2 top_left_screen = from_ndc( text.box_top_left, target_size );
        const Float2 bottom_right_screen = from_ndc( text.box_bottom_right, target_size );

        layout_rect.left = top_left_screen.x;
        layout_rect.top = top_left_screen.y;
        layout_rect.right = bottom_right_screen.x;
        layout_rect.bottom = bottom_right_screen.y;

        text.format->SetTextAlignment( text.h_align );
        text.format->SetParagraphAlignment( text.v_align );

        target->CreateSolidColorBrush(
            D2D1_COLOR_F{
                .r = text.color.x,
                .g = text.color.y,
                .b = text.color.z,
                .a = text.color.w,
            },
            &brush );

        target->DrawTextW(
            text.data.data(),
            (UINT) text.data.size(),
            text.format.get(),
            layout_rect,
            brush.get() );
    }
    target->EndDraw();
}

void kl::TextRaster::draw_text_direct( UINT target_index, Text const& text ) const
{
    if ( !text.format )
        return;

    auto const& target = m_d2d1_targets[target_index];
    const Float2 target_size = (Float2 const&) target->GetSize();

    const Float2 top_left_screen = from_ndc( text.box_top_left, target_size );
    const Float2 bottom_right_screen = from_ndc( text.box_bottom_right, target_size );

    const D2D1_RECT_F layout_rect{
        .left = top_left_screen.x,
        .top = top_left_screen.y,
        .right = bottom_right_screen.x,
        .bottom = bottom_right_screen.y,
    };

    text.format->SetTextAlignment( text.h_align );
    text.format->SetParagraphAlignment( text.v_align );

    target->BeginDraw();

    ComRef<ID2D1SolidColorBrush> brush;
    target->CreateSolidColorBrush(
        D2D1_COLOR_F{
            .r = text.color.x,
            .g = text.color.y,
            .b = text.color.z,
            .a = text.color.w,
        },
        &brush );

    target->DrawTextW(
        text.data.data(),
        (UINT) text.data.size(),
        text.format.get(),
        layout_rect,
        brush.get() );

    target->EndDraw();
}

#pragma once

#include "apis/apis.h"


namespace kl
{
inline constexpr UINT GPU_BUFFER_COUNT = 2;
}

namespace kl
{
using HAlign = DWRITE_TEXT_ALIGNMENT;
using VAlign = DWRITE_PARAGRAPH_ALIGNMENT;

struct Text
{
    TextFormat format;
    kl::float4 color{ 1.0f };
    kl::float2 box_top_left;
    kl::float2 box_bottom_right;
    std::wstring data;
    HAlign h_align = DWRITE_TEXT_ALIGNMENT_LEADING;
    VAlign v_align = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
};
}

namespace kl
{
struct TextRaster : NoCopy
{
    std::vector<Text> text_batch;

    TextRaster();

    TextFormat create_text_format(
        std::wstring_view const& font_family,
        DWRITE_FONT_WEIGHT font_weight,
        DWRITE_FONT_STYLE font_style,
        float font_size,
        std::wstring_view const& locale = L"en-us"
    ) const;

    void draw_text_batch( UINT target_index ) const;
    void draw_text_direct( UINT target_index, Text const& text ) const;

protected:
    ComRef<ID2D1Factory> m_d2d1_factory;
    ComRef<IDWriteFactory> m_write_factory;
    ComRef<ID2D1RenderTarget> m_d2d1_targets[GPU_BUFFER_COUNT];
};
}

#pragma once

#include "math/math.h"
#include "memory/safety/ref.h"

namespace kl
{
inline constexpr UINT GPU_BUFFER_COUNT = 2;
}

namespace kl
{
using HAlign = DWRITE_TEXT_ALIGNMENT;
using VAlign = DWRITE_PARAGRAPH_ALIGNMENT;

struct RasterItem
{
    virtual ~RasterItem() noexcept = default;
    virtual void draw(ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush) const = 0;
};

struct RasterText : RasterItem
{
    TextFormat format;
    std::wstring data;
    float4 color{1.0f};
    float2 box_top_left;
    float2 box_bottom_right;
    HAlign h_align = DWRITE_TEXT_ALIGNMENT_LEADING;
    VAlign v_align = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

    RasterText() = default;
    RasterText(TextFormat const& format, std::string_view data, float4 color, float2 tl, float2 br = {1.0f, -1.0f},
               HAlign h_align = DWRITE_TEXT_ALIGNMENT_LEADING, VAlign v_align = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    RasterText(TextFormat const& format, std::wstring_view data, float4 color, float2 tl, float2 br = {1.0f, -1.0f},
               HAlign h_align = DWRITE_TEXT_ALIGNMENT_LEADING, VAlign v_align = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    void draw(ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush) const override;
};

struct RasterLine : RasterItem
{
    float2 start;
    float2 end;
    float4 color{1.0f};
    float stroke_width = 1.0f;

    RasterLine() = default;
    RasterLine(float2 start, float2 end, float4 color, float stroke_width = 1.0f);

    void draw(ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush) const override;
};

struct RasterRectangle : RasterItem
{
    float2 top_left;
    float2 bottom_right;
    float4 color{1.0f};
    bool fill = false;
    float2 corner_radius;
    float stroke_width = 1.0f;

    RasterRectangle() = default;
    RasterRectangle(float2 const& top_left, float2 const& bottom_right, float4 color, bool fill,
                    float2 const& corner_radius = {}, float stroke_width = 1.0f);

    void draw(ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush) const override;
};

struct RasterEllipse : RasterItem
{
    float2 center;
    float2 radius;
    float4 color{1.0f};
    bool fill = false;
    float stroke_width = 1.0f;

    RasterEllipse() = default;
    RasterEllipse(float2 const& center, float2 const& radius, float4 color, bool fill, float stroke_width = 1.0f);

    void draw(ComRef<ID2D1RenderTarget> const& target, ComRef<ID2D1SolidColorBrush> const& brush) const override;
};
} // namespace kl

namespace kl
{
struct Raster : NoCopy
{
    std::vector<Ref<RasterItem>> raster_batch;

    Raster();

    TextFormat create_text_format(std::wstring_view font_family, DWRITE_FONT_WEIGHT font_weight,
                                  DWRITE_FONT_STYLE font_style, float font_size,
                                  std::wstring_view locale = L"en-us") const;

    void draw_raster_batch(UINT target_index) const;
    void draw_raster_direct(UINT target_index, RasterItem const& raster_item) const;

  protected:
    ComRef<ID2D1Factory> m_d2d1_factory;
    ComRef<IDWriteFactory> m_write_factory;
    ComRef<ID2D1RenderTarget> m_d2d1_targets[GPU_BUFFER_COUNT];
};
} // namespace kl

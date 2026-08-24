#include "examples.h"

int examples::text_drawing_main(int argc, char** argv)
{
    kl::Window window{"Text Drawing"};
    kl::GPU gpu{window.ptr()};

    window.on_resize.emplace_back([&](int2 size) {
        gpu.resize_internal(size);
        gpu.set_viewport_size(size);
    });
    window.set_dark_mode(true);
    window.maximize();

    kl::TextFormat format =
        gpu.create_text_format(L"roboto", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, 25.0f);
    if (!format)
        return 1;

    std::wstringstream message;
    for (int i = 0; i < 20; i++)
        message << i << "^2 = " << i * i << '\n';

    kl::Ref<kl::RasterText> text = new kl::RasterText();
    text->format = format;
    text->box_top_left = {-1.0f, 1.0f};
    text->box_bottom_right = {1.0f, -1.0f};
    text->h_align = kl::HAlign::DWRITE_TEXT_ALIGNMENT_CENTER;
    text->v_align = kl::VAlign::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
    text->data = message.str();
    gpu.raster_batch.push_back(text);

    while (window.process())
    {
        gpu.clear_internal(kl::colors::GRAY);
        gpu.draw_raster_batch();
        gpu.swap_buffers(true);
    }
    return 0;
}

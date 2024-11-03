#include "examples.h"


int examples::text_drawing_main(const int argc, const char** argv)
{
	kl::Window window{ "Text Drawing" };
	kl::GPU gpu{ window.ptr() };

	window.on_resize.emplace_back([&](kl::Int2 size)
	{
		gpu.resize_internal(size);
		gpu.set_viewport_size(size);
	});
	window.set_dark_mode(true);
	window.maximize();

	kl::TextFormat format = gpu.create_text_format(L"roboto", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, 25.0f);
	kl::assert(format, "failed to init text format");

	while (window.process()) {
		gpu.clear_internal(kl::colors::GRAY);

		gpu.text_data.resize(20);
		for (int i = 0; i < (int) gpu.text_data.size(); i++) {
			auto& text = gpu.text_data[i];
			text.format = format;
			text.position = { window.size().x * 0.5f, (i + 1) * 50.0f };
			text.data = kl::convert_string(kl::format(i, "^2 = ", i * i));
		}

		gpu.draw_text();
		gpu.swap_buffers(true);
	}
	return 0;
}

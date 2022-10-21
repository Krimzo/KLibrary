#include "klib.h"


static constexpr float fps_limit = 165.0f;

int main()
{
	kl::window window = { { 900, 900 }, "Interpolation" };
	auto frame = kl::image(window.size());
	kl::timer timer;

	kl::float2 position_a = { 50.0f, (frame.height() - 1.0f) / 3.0f };
	kl::float2 position_b = { (frame.width() - 1.0f) / 3.0f, frame.height() - 51.0f };
	kl::float2 position_c = { frame.width() - 51.0f, (frame.height() - 1.0f) * 0.667f };
	kl::float2 position_d = { (frame.width() - 1.0f) * 0.667f, 50.0f };

	kl::color color_a = kl::colors::red;
	kl::color color_b = kl::colors::white;
	kl::color color_c = kl::colors::blue;
	kl::color color_d = kl::colors::green;

	kl::triangle triangle_1 = {
		kl::vertex(kl::float3(position_a, 0.5f)), kl::vertex(kl::float3(position_b, 0.5f)),
		kl::vertex(kl::float3(position_c, 0.5f))
	};
	kl::triangle triangle_2 = {
		kl::vertex(kl::float3(position_a, 0.5f)), kl::vertex(kl::float3(position_d, 0.5f)),
		kl::vertex(kl::float3(position_c, 0.5f))
	};

	const kl::float4 t1_constants = triangle_1.get_constants();
	const kl::float4 t2_constants = triangle_2.get_constants();

	uint32_t frame_index = 0;
	while (window.process(false)) {
		timer.reset();

		for (uint32_t x = frame_index - frame.height(), y = 0; y < frame.height(); x++, y++) {
			const kl::float3 t1_weights = triangle_1.get_weights(t1_constants, { x, y });
			const kl::float3 t2_weights = triangle_2.get_weights(t2_constants, { x, y });

			kl::color pixel;
			if (kl::triangle::is_in_triangle(t1_weights)) {
				pixel.r = static_cast<uint8_t>(kl::triangle::interpolate(t1_weights, {color_a.r, color_b.r, color_c.r}));
				pixel.g = static_cast<uint8_t>(kl::triangle::interpolate(t1_weights, { color_a.g, color_b.g, color_c.g }));
				pixel.b = static_cast<uint8_t>(kl::triangle::interpolate(t1_weights, { color_a.b, color_b.b, color_c.b }));
			}
			else if (kl::triangle::is_in_triangle(t2_weights)) {
				pixel.r = static_cast<uint8_t>(kl::triangle::interpolate(t2_weights, { color_a.r, color_d.r, color_c.r }));
				pixel.g = static_cast<uint8_t>(kl::triangle::interpolate(t2_weights, { color_a.g, color_d.g, color_c.g }));
				pixel.b = static_cast<uint8_t>(kl::triangle::interpolate(t2_weights, { color_a.b, color_d.b, color_c.b }));
			}
			else {
				pixel = kl::colors::gray;
			}

			frame.set_pixel({ x, y }, pixel);
			frame.set_pixel({ x + 1, y }, kl::random::get_color());
			frame.set_pixel({ x + 2, y }, kl::random::get_color());
		}

		window.draw_image(frame);

		window.set_title(kl::format(static_cast<int>((100.0f * frame_index) / (frame.width() + frame.height() - 1)), "%"));

		kl::time::wait((1.0f / fps_limit) - timer.get_elapsed());

		if (++frame_index == frame.width() + frame.height()) {
			window.set_title("Finished!");
			while (window.process());
		}
	}
}

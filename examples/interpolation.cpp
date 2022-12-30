#include "klib.h"


static constexpr float fps_limit = 165.0f;

int main()
{
	kl::window window = { { 900, 900 }, "Interpolation" };
	kl::image frame = kl::image(window.size());
	kl::timer timer = {};

	kl::float2 positions[4] = {
		{ 50.0f, (frame.height() - 1.0f) / 3.0f },
		{ (frame.width() - 1.0f) / 3.0f, frame.height() - 51.0f },
		{ frame.width() - 51.0f, (frame.height() - 1.0f) * 0.667f },
		{ (frame.width() - 1.0f) * 0.667f, 50.0f },
	};

	kl::color colors[4] = {
		kl::colors::red,
		kl::colors::white,
		kl::colors::blue,
		kl::colors::green,
	};

	kl::triangle triangles[2] = {
		kl::triangle {
			kl::vertex(kl::float3(positions[0], 0.5f)),
			kl::vertex(kl::float3(positions[1], 0.5f)),
			kl::vertex(kl::float3(positions[2], 0.5f)),
		},
		kl::triangle {
			kl::vertex(kl::float3(positions[0], 0.5f)),
			kl::vertex(kl::float3(positions[3], 0.5f)),
			kl::vertex(kl::float3(positions[2], 0.5f)),
		},
	};

	kl::float4 constants[2] = {
		triangles[0].get_constants(),
		triangles[1].get_constants(),
	};

	int frame_index = 0;
	while (window.process(false)) {
		timer.reset();

		for (int x = frame_index - frame.height(), y = 0; y < frame.height(); x++, y++) {
			kl::float3 weights[2] = {
				triangles[0].get_weights(constants[0], { x, y }),
				triangles[1].get_weights(constants[1], { x, y }),
			};

			kl::color pixel = {};
			if (kl::triangle::is_in_triangle(weights[0])) {
				pixel.r = (uint8_t) kl::triangle::interpolate(weights[0], { colors[0].r, colors[1].r, colors[2].r });
				pixel.g = (uint8_t) kl::triangle::interpolate(weights[0], { colors[0].g, colors[1].g, colors[2].g });
				pixel.b = (uint8_t) kl::triangle::interpolate(weights[0], { colors[0].b, colors[1].b, colors[2].b });
			}
			else if (kl::triangle::is_in_triangle(weights[1])) {
				pixel.r = (uint8_t) kl::triangle::interpolate(weights[1], { colors[0].r, colors[3].r, colors[2].r });
				pixel.g = (uint8_t) kl::triangle::interpolate(weights[1], { colors[0].g, colors[3].g, colors[2].g });
				pixel.b = (uint8_t) kl::triangle::interpolate(weights[1], { colors[0].b, colors[3].b, colors[2].b });
			}
			else {
				pixel = kl::colors::gray;
			}

			frame.set_pixel({ x + 0, y }, pixel);
			frame.set_pixel({ x + 1, y }, kl::random::get_color());
			frame.set_pixel({ x + 2, y }, kl::random::get_color());
		}

		window.draw_image(frame);

		window.set_title(kl::format((int) ((100.0f * frame_index) / (frame.width() + frame.height() - 1)), "%"));

		kl::time::wait((1.0f / fps_limit) - timer.get_elapsed());

		if (++frame_index == frame.width() + frame.height()) {
			window.set_title("Finished!");
			while (window.process());
		}
	}
}

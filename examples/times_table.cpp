#include "klib.h"


static constexpr int n = 200;
static constexpr float increment = 0.15f;
static float m = 0.0f;

int main()
{
	const kl::color circle_color = kl::colors::console;
	kl::color point_color = kl::random::get_color();
	kl::color line_color = kl::random::get_color();

	kl::window window = { { 900, 900 }, "Times Table" };
	kl::image frame(window.size(), kl::colors::gray);

	window.on_resize = [&](const kl::uint2& size) {
		if (size.x > 0 && size.y > 0) {
			frame.set_size(size);
		}
	};

	window.keyboard.r.on_press = [&] {
		point_color = kl::random::get_color();
		line_color = kl::random::get_color();
	};

	while (window.process(false)) {
		m += increment * kl::time::get_interval();

		frame.fill(kl::colors::gray);

		const float circle_radius = std::min(static_cast<float>(frame.width()) * 0.5f, static_cast<float>(frame.height()) * 0.5f) - 25.0f;
		frame.draw_circle(kl::int2(frame.size() / 2), circle_radius, circle_color);

		static constexpr float point_angle = 360.0f / n;
		std::vector<kl::int2> circle_points(n);
		for (int i = 0; i < static_cast<int>(circle_points.size()); i++) {
			circle_points[i] = {
				static_cast<int>(std::cos(kl::math::to_radians(point_angle * i + 180.0f)) * circle_radius + frame.width() / 2),
				static_cast<int>(std::sin(kl::math::to_radians(point_angle * i + 180.0f)) * circle_radius + frame.height() / 2)
			};
		}

		for (auto& point : circle_points) {
			frame.draw_circle(point, 3.0f, point_color, true);
		}

		for (int i = 0; i < static_cast<int>(circle_points.size()); i++) {
			frame.draw_line(circle_points[i], circle_points[std::abs(static_cast<int64_t>(i * m) % n)], line_color);
		}

		window.draw_image(frame);

		window.set_title(kl::format("n: ", n, " m: ", m));
	}
}

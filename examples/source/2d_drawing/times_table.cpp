#include "examples.h"


static constexpr int n = 200;
static constexpr float increment = 0.15f;
static float m = 0.0f;

int examples::times_table_main(const int argc, const char** argv)
{
    const kl::Color circle_color = kl::colors::CONSOLE;
    kl::Color point_color = kl::random::gen_color();
    kl::Color line_color = kl::random::gen_color();
    
    kl::Window window{ "Times Table" };
    kl::Image frame;

    window.resize({ 900, 900 });
    frame.resize(window.size());

    window.on_resize.push_back([&](const kl::Int2& size)
    {
        if (size.x > 0 && size.y > 0) {
            frame.resize(size);
        }
    });

    while (window.process()) {
        m += increment * kl::time::delta();

        if (window.keyboard.r.pressed()) {
            point_color = kl::random::gen_color();
            line_color = kl::random::gen_color();
        }

        frame.fill(kl::colors::GRAY);

        const float circle_radius = std::min(frame.width() * 0.5f, frame.height() * 0.5f) - 25.0f;
        frame.draw_circle(frame.size() / 2, circle_radius, circle_color);

        static constexpr float point_angle = 360.0f / n;
        std::vector<kl::Int2> circle_points(n);
        for (int i = 0; i < int(circle_points.size()); i++) {
            circle_points[i] = {
                (int) (kl::cos_d(point_angle * i + 180.0f) * circle_radius + frame.width() * 0.5f),
                (int) (kl::sin_d(point_angle * i + 180.0f) * circle_radius + frame.height() * 0.5f),
            };
        }

        for (auto& point : circle_points) {
            frame.draw_circle(point, 3.0f, point_color, true);
        }

        for (int i = 0; i < int(circle_points.size()); i++) {
            frame.draw_line(circle_points[i], circle_points[std::abs(int(i * m) % n)], line_color);
        }

        window.draw_image(frame);

        window.set_title(kl::format("n: ", n, " m: ", m));
    }
    return 0;
}

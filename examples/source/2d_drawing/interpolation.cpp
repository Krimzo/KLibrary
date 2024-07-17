#include "examples.h"


static constexpr float fps_limit = 165.0f;

int examples::interpolation_main()
{
    auto window = kl::Window("Interpolation", { 900, 900 });
    kl::Image frame = kl::Image(window.size());
    kl::Timer timer = {};

    kl::Float2 positions[4] = {
        {                           50.0f,   (frame.height() - 1.0f) / 3.0f },
        {   (frame.width() - 1.0f) / 3.0f,           frame.height() - 51.0f },
        {           frame.width() - 51.0f, (frame.height() - 1.0f) * 0.667f },
        { (frame.width() - 1.0f) * 0.667f,                            50.0f },
    };

    kl::Color colors[4] = {
        kl::colors::RED,
        kl::colors::WHITE,
        kl::colors::BLUE,
        kl::colors::GREEN,
    };

    kl::Triangle<float> triangles[2] = {
        kl::Triangle {
            kl::Vertex(kl::Float3(positions[0], 0.5f)),
            kl::Vertex(kl::Float3(positions[1], 0.5f)),
            kl::Vertex(kl::Float3(positions[2], 0.5f)),
        },
        kl::Triangle {
            kl::Vertex(kl::Float3(positions[0], 0.5f)),
            kl::Vertex(kl::Float3(positions[3], 0.5f)),
            kl::Vertex(kl::Float3(positions[2], 0.5f)),
        },
    };

    kl::Float4 constants[2] = {
        triangles[0].constants(),
        triangles[1].constants(),
    };

    int frame_index = 0;
    while (window.process(false)) {
        timer.reset_elapsed();

        for (int x = frame_index - frame.height(), y = 0; y < frame.height(); x++, y++) {
            kl::Float3 weights[2] = {
                triangles[0].weights(constants[0], { (float) x, (float) y }),
                triangles[1].weights(constants[1], { (float) x, (float) y }),
            };

            kl::Color pixel = {};
            if (kl::Triangle<float>::is_in_triangle(weights[0])) {
                pixel.r = (byte) kl::Triangle<float>::interpolate(weights[0], { (float) colors[0].r, (float) colors[1].r, (float) colors[2].r });
                pixel.g = (byte) kl::Triangle<float>::interpolate(weights[0], { (float) colors[0].g, (float) colors[1].g, (float) colors[2].g });
                pixel.b = (byte) kl::Triangle<float>::interpolate(weights[0], { (float) colors[0].b, (float) colors[1].b, (float) colors[2].b });
            }
            else if (kl::Triangle<float>::is_in_triangle(weights[1])) {
                pixel.r = (byte) kl::Triangle<float>::interpolate(weights[1], { (float) colors[0].r, (float) colors[3].r, (float) colors[2].r });
                pixel.g = (byte) kl::Triangle<float>::interpolate(weights[1], { (float) colors[0].g, (float) colors[3].g, (float) colors[2].g });
                pixel.b = (byte) kl::Triangle<float>::interpolate(weights[1], { (float) colors[0].b, (float) colors[3].b, (float) colors[2].b });
            }
            else {
                pixel = kl::colors::GRAY;
            }

            if (kl::Int2 write_position = { x + 0, y }; frame.in_bounds(write_position)) {
                frame[write_position] = pixel;
            }
            if (kl::Int2 write_position = { x + 1, y }; frame.in_bounds(write_position)) {
                frame[write_position] = kl::random::gen_color();
            }
            if (kl::Int2 write_position = { x + 2, y }; frame.in_bounds(write_position)) {
                frame[write_position] = kl::random::gen_color();
            }
        }

        window.draw_image(frame);
        window.set_title(kl::format((int) ((100.0f * frame_index) / (frame.width() + frame.height() - 1)), "%"));
        kl::time::wait((1.0f / fps_limit) - timer.elapsed());

        if (++frame_index == frame.width() + frame.height()) {
            window.set_title("Finished!");
            while (window.process());
        }
    }
    return 0;
}

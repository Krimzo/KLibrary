#include "examples.h"


static constexpr float fps_limit = 165.0f;

int examples::interpolation_main()
{
    auto window = kl::window({ 900, 900 }, "Interpolation");
    kl::image frame = kl::image(window.size());
    kl::timer timer = {};

    kl::float2 positions[4] = {
        {                           50.0f,   (frame.height() - 1.0f) / 3.0f },
        {   (frame.width() - 1.0f) / 3.0f,           frame.height() - 51.0f },
        {           frame.width() - 51.0f, (frame.height() - 1.0f) * 0.667f },
        { (frame.width() - 1.0f) * 0.667f,                            50.0f },
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
        triangles[0].constants(),
        triangles[1].constants(),
    };

    int frame_index = 0;
    while (window.process(false)) {
        timer.reset();

        for (int x = frame_index - frame.height(), y = 0; y < frame.height(); x++, y++) {
            kl::float3 weights[2] = {
                triangles[0].weights(constants[0], { (float) x, (float) y }),
                triangles[1].weights(constants[1], { (float) x, (float) y }),
            };

            kl::color pixel = {};
            if (kl::triangle::is_in_triangle(weights[0])) {
                pixel.r = (byte) kl::triangle::interpolate(weights[0], { (float) colors[0].r, (float) colors[1].r, (float) colors[2].r });
                pixel.g = (byte) kl::triangle::interpolate(weights[0], { (float) colors[0].g, (float) colors[1].g, (float) colors[2].g });
                pixel.b = (byte) kl::triangle::interpolate(weights[0], { (float) colors[0].b, (float) colors[1].b, (float) colors[2].b });
            }
            else if (kl::triangle::is_in_triangle(weights[1])) {
                pixel.r = (byte) kl::triangle::interpolate(weights[1], { (float) colors[0].r, (float) colors[3].r, (float) colors[2].r });
                pixel.g = (byte) kl::triangle::interpolate(weights[1], { (float) colors[0].g, (float) colors[3].g, (float) colors[2].g });
                pixel.b = (byte) kl::triangle::interpolate(weights[1], { (float) colors[0].b, (float) colors[3].b, (float) colors[2].b });
            }
            else {
                pixel = kl::colors::gray;
            }

            if (kl::int2 write_position = { x + 0, y }; frame.in_bounds(write_position)) {
                frame[write_position] = pixel;
            }
            if (kl::int2 write_position = { x + 1, y }; frame.in_bounds(write_position)) {
                frame[write_position] = kl::random::gen_color();
            }
            if (kl::int2 write_position = { x + 2, y }; frame.in_bounds(write_position)) {
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

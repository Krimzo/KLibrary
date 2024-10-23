#include "examples.h"


static int ITERATIONS = 64;
static float ZOOM = 1.0f;
static kl::Float2 POSITION = { -0.5f, 0.0f };
static kl::RGB START_COLOR = kl::colors::ORANGE;

static void input(const kl::Window& window, const float delta_time)
{
    if (window.keyboard.esc) {
        window.close();
    }
    if (window.keyboard.r) {
        if (window.keyboard.shift) {
            ITERATIONS = 64;
        }
        else {
            POSITION = { -0.5, 0.0f };
            ZOOM = 1.0f;
        }
    }
    if (window.keyboard.w) {
        POSITION.y -= (1.0f / ZOOM) * delta_time;
    }
    if (window.keyboard.s) {
        POSITION.y += (1.0f / ZOOM) * delta_time;
    }
    if (window.keyboard.d) {
        POSITION.x += (1.0f / ZOOM) * delta_time;
    }
    if (window.keyboard.a) {
        POSITION.x -= (1.0f / ZOOM) * delta_time;
    }

    const kl::Int2 frame_size = window.size();
    if (window.mouse.left) {
        ZOOM += ZOOM * delta_time;
        kl::Float2 uv = {
            window.mouse.position().x / (float) frame_size.x * 2.0f - 1.0f,
            window.mouse.position().y / (float) frame_size.y * 2.0f - 1.0f,
        };
        uv *= (float) frame_size.x / (float) frame_size.y;
        POSITION += (uv * (1.0f / ZOOM)) * delta_time;
    }
    if (window.mouse.right) {
        ZOOM -= ZOOM * delta_time;
        kl::Float2 uv = {
            window.mouse.position().x / (float) frame_size.x * 2.0f - 1.0f,
            window.mouse.position().y / (float) frame_size.y * 2.0f - 1.0f,
        };
        uv *= (float) frame_size.x / (float) frame_size.y;
        POSITION -= (uv * (1.0f / ZOOM)) * delta_time;
    }

    ITERATIONS += window.mouse.scroll() * (window.keyboard.shift ? 10 : 1);
    ITERATIONS = std::max(ITERATIONS, 0);
}

static void console_read()
{
    while (true) {
        kl::print<false>(kl::colors::CONSOLE, "Color = ");
        if (std::vector<std::string> parts = kl::split_string([]
        {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }(), ' '); parts.size() >= 3) {
            try {
                kl::RGB result_color;
                result_color.r = (byte) kl::parse_int(parts[0]).value_or(0);
                result_color.g = (byte) kl::parse_int(parts[1]).value_or(0);
                result_color.b = (byte) kl::parse_int(parts[2]).value_or(0);
                START_COLOR = result_color;
                print(START_COLOR, "Color updated!");
            }
            catch (std::exception&) {
                kl::print(kl::colors::RED, "Bad input");
            }
        }
        else {
            kl::print(kl::colors::ORANGE, "Not enough data");
        }
    }
}

int examples::mandelbrot_main(const int argc, const char** argv)
{
    kl::Window window{ "Mandelbrot" };
    kl::GPU gpu{ window };
    kl::Timer timer;

    window.on_resize.emplace_back([&](kl::Int2 size)
    {
        gpu.resize_internal(size);
        gpu.set_viewport_size(size);
    });
    window.maximize();

    std::string shader_sources = kl::read_file("shaders/mandelbrot.hlsl");
    kl::Shaders shaders = gpu.create_shaders(shader_sources);
    kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();

    std::thread(console_read).detach();

    while (window.process()) {
        timer.update();

        input(window, timer.delta());

        gpu.clear_internal();

        struct alignas(16) CB
        {
            kl::Float4 STATE_INFO;
            kl::Float4 FRAME_SIZE;
            kl::Float4 START_COLOR;
        } cb = {};

        cb.STATE_INFO = { POSITION, ZOOM, (float) ITERATIONS };
        cb.FRAME_SIZE = { window.size(), {} };
        cb.START_COLOR = START_COLOR;

        shaders.upload(cb);
        gpu.bind_shaders(shaders);

        gpu.draw(screen_mesh);
        gpu.swap_buffers(true);

        window.set_title(kl::format(
            "(Iterations: ", ITERATIONS, ") ",
            "(Zoom: ", std::fixed, std::setprecision(2), ZOOM, ") ",
            "(Position: ", POSITION, ")"
        ));
    }
    return 0;
}

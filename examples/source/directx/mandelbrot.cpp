#include "klib.h"


struct ps_cb
{
    kl::float4 state_info;
    kl::float4 frame_size;
    kl::float4 start_color;
};

static int iterations = 64;
static float zoom = 1.0f;
static kl::float2 position = { -0.5f, 0.0f };
static kl::color start_color = kl::colors::orange;

static void input(const kl::window& window, const float delta_time)
{
    // Keyboard
    if (window.keyboard.esc) {
        window.close();
    }
    if (window.keyboard.r) {
        if (window.keyboard.shift) {
            iterations = 64;
        }
        else {
            position = { -0.5, 0.0f };
            zoom = 1.0f;
        }
    }
    if (window.keyboard.w) {
        position.y -= (1.0f / zoom) * delta_time;
    }
    if (window.keyboard.s) {
        position.y += (1.0f / zoom) * delta_time;
    }
    if (window.keyboard.d) {
        position.x += (1.0f / zoom) * delta_time;
    }
    if (window.keyboard.a) {
        position.x -= (1.0f / zoom) * delta_time;
    }

    // Mouse
    const kl::int2 frame_size = window.size();
    if (window.mouse.left) {
        zoom += zoom * delta_time;
        kl::float2 uv = {
            window.mouse.position().x / (float) frame_size.x * 2.0f - 1.0f,
            window.mouse.position().y / (float) frame_size.y * 2.0f - 1.0f,
        };

        uv *= float(frame_size.x) / frame_size.y;
        position += (uv * (1.0f / zoom)) * delta_time;
    }
    if (window.mouse.right) {
        zoom -= zoom * delta_time;
        kl::float2 uv = {
            window.mouse.position().x / (float) frame_size.x * 2.0f - 1.0f,
            window.mouse.position().y / (float) frame_size.y * 2.0f - 1.0f,
        };

        uv *= float(frame_size.x) / frame_size.y;
        position -= (uv * (1.0f / zoom)) * delta_time;
    }

    // Scroll
    static int last_scroll = window.mouse.scroll();
    const int current_scroll = window.mouse.scroll();
    iterations += (current_scroll - last_scroll) * (window.keyboard.shift ? 10 : 1);
    iterations = max(iterations, 0);
    last_scroll = current_scroll;
}

static void console_read()
{
    while (true) {
        kl::print<false>(kl::colors::console, "Color = ");

        if (std::vector<std::string> parts = kl::strings::split([]
        {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }(), ' '); parts.size() >= 3) {
            try {
                kl::color result_color = {};
                result_color.r = uint8_t(std::stoi(parts[0]));
                result_color.g = uint8_t(std::stoi(parts[1]));
                result_color.b = uint8_t(std::stoi(parts[2]));
                start_color = result_color;
                print(start_color, "Color updated!");
            }
            catch (std::exception&) {
                kl::print(kl::colors::red, "Bad input");
            }
        }
        else {
            kl::print(kl::colors::orange, "Not enough data");
        }
    }
}

int main()
{
    kl::BOUND_WINDOW = kl::window::make({ 1600, 900 }, "Mandelbrot");
    kl::BOUND_GPU = kl::gpu::make(*kl::BOUND_WINDOW);
    kl::timer timer = {};

    auto& window = *kl::BOUND_WINDOW;
    auto& gpu = *kl::BOUND_GPU;

    window.on_resize.push_back([&](const kl::int2 size)
    {
        if (size.x > 0 && size.y > 0) {
            gpu.resize_internal(size);
            gpu.set_viewport(size);
        }
    });

    window.maximize();

    // Start
    auto raster_state = kl::gpu_raster_state::make(false, false, true);
    raster_state->bind();

    auto shaders = kl::gpu_shaders::make(kl::files::read_string("examples/shaders/mandelbrot.hlsl"));
    shaders->bind();

    auto const_buffer = kl::gpu_const_buffer::make(sizeof(ps_cb));
    const_buffer->bind_for_pixel_shader(0);

    auto screen_mesh = kl::gpu_mesh::make_screen();

    // Console
    std::thread(console_read).detach();

    // Update
    while (window.process(false)) {
        timer.update_interval();

        // Input
        input(window, timer.get_interval());

        // Render
        gpu.clear_internal();

        ps_cb ps_data = {};
        ps_data.state_info = { position, zoom, (float) iterations };
        ps_data.frame_size = { window.size(), 0.0f, 0.0f };
        ps_data.start_color = start_color;
        const_buffer->set_data(ps_data);

        screen_mesh->draw();

        gpu.swap_buffers(true);

        // Info
        window.set_title(kl::format(
            "[", std::setw(3), int(1 / timer.get_interval()), "] ",
            "(Iterations: ", iterations, ") ",
            "(Zoom: ", std::fixed, std::setprecision(2), zoom, ") ",
            "(Position: ", position, ")"
        ));
    }
}
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
    if (window.keyboard.esc.state()) {
        window.close();
    }
    if (window.keyboard.r.state()) {
        if (window.keyboard.shift.state()) {
            iterations = 64;
        }
        else {
            position = {-0.5, 0.0f};
            zoom = 1.0f;
        }
    }
    if (window.keyboard.w.state()) {
        position.y -= (1.0f / zoom) * delta_time;
    }
    if (window.keyboard.s.state()) {
        position.y += (1.0f / zoom) * delta_time;
    }
    if (window.keyboard.d.state()) {
        position.x += (1.0f / zoom) * delta_time;
    }
    if (window.keyboard.a.state()) {
        position.x -= (1.0f / zoom) * delta_time;
    }

    // Mouse
    const kl::int2 frame_size = window.size();
    if (window.mouse.left.state()) {
        zoom += zoom * delta_time;
        kl::float2 uv = kl::float2(window.mouse.position()) / kl::float2(frame_size) * 2.0f - kl::float2::splash(1);
        uv *= float(frame_size.x) / frame_size.y;
        position += (uv / zoom) * delta_time;
    }
    if (window.mouse.right.state()) {
        zoom -= zoom * delta_time;
        kl::float2 uv = kl::float2(window.mouse.position()) / kl::float2(frame_size) * 2.0f - kl::float2::splash(1);
        uv *= float(frame_size.x) / frame_size.y;
        position -= (uv / zoom) * delta_time;
    }

    // Scroll
    static int last_scroll = window.mouse.scroll();
    const int current_scroll = window.mouse.scroll();
    iterations += (current_scroll - last_scroll) * (window.keyboard.shift.state() ? 10 : 1);
    iterations = std::max(iterations, 0);
    last_scroll = current_scroll;
}

[[noreturn]] static void console_read()
{
    while (true) {
        kl::print<false>(kl::colors::console, "Color = ");

        if (std::vector<std::string> parts = kl::strings::split([] {
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
    kl::window window = { { 1600, 900 }, "Mandelbrot" };
    kl::gpu gpu = kl::gpu(window.get_window());
    kl::timer timer = {};

    window.on_resize = [&](const kl::int2 size)
    {
        if (size.x > 0 && size.y > 0) {
            gpu.resize_internal(size);
            gpu.set_viewport(size);
        }
    };
    window.maximize();

    // Start
    gpu.bind_shaders(gpu.new_shaders(kl::files::read_string("examples/shaders/mandelbrot.hlsl")));
    const kl::dx::buffer screen_mesh = gpu.generate_screen_mesh();

    // Console
    std::thread(console_read).detach();

    // Update
    while (window.process(false)) {
        timer.update_interval();

        // Input
        input(window, timer.get_interval());

        // Render
        ps_cb ps_data = {};
        ps_data.state_info.xy = position;
        ps_data.state_info.z = zoom;
        ps_data.state_info.w = float(iterations);
        ps_data.frame_size.xy = kl::float2(window.size());
        ps_data.start_color = kl::float4(start_color);

        gpu.clear_internal();
        gpu.set_pixel_const_buffer(ps_data);
        gpu.draw_vertex_buffer(screen_mesh);
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

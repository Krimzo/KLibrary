#include "klib.h"

using namespace kl;


struct ps_cb
{
    float4 state_info;
    float4 frame_size;
    float4 start_color;
};

static int iterations = 64;
static float zoom = 1.0f;
static float2 position = {-0.5f, 0.0f};
static color start_color = colors::orange;

static void input(const window& window, const float delta_time)
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
    const auto frame_size = int2(window.size());
    if (window.mouse.left.state()) {
        zoom += zoom * delta_time;
        float2 uv = float2(window.mouse.position()) / float2(frame_size) * 2.0f - float2::splash(1);
        uv *= static_cast<float>(frame_size.x) / static_cast<float>(frame_size.y);
        position += (uv / zoom) * delta_time;
    }
    if (window.mouse.right.state()) {
        zoom -= zoom * delta_time;
        float2 uv = float2(window.mouse.position()) / float2(frame_size) * 2.0f - float2::splash(1);
        uv *= static_cast<float>(frame_size.x) / static_cast<float>(frame_size.y);
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
        print<false>(colors::console, "Color = ");

        if (std::vector<std::string> parts = strings::split([]
        {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }(), ' '); parts.size() >= 3) {
            try {
                color result_color = {};
                result_color.r = static_cast<uint8_t>(std::stoi(parts[0]));
                result_color.g = static_cast<uint8_t>(std::stoi(parts[1]));
                result_color.b = static_cast<uint8_t>(std::stoi(parts[2]));
                start_color = result_color;
                print(start_color, "Color updated!");
            }
            catch (std::exception&) {
                print(colors::red, "Bad input");
            }
        }
        else {
            print(colors::orange, "Not enough data");
        }
    }
}

int main()
{
    window window = {uint2{1600, 900}, "Mandelbrot"};
    auto gpu = kl::gpu(window.get_window());
    timer timer = {};

    window.on_resize = [&](const uint2 size)
    {
        if (size.x > 0 && size.y > 0) {
            gpu.resize_internal(size);
            gpu.set_viewport(size);
        }
    };
    window.maximize();

    // Start
    gpu.bind_shaders(gpu.new_shaders(files::read_string("Examples/Shaders/Mandelbrot.hlsl")));
    const dx::buffer screen_mesh = gpu.generate_screen_mesh();

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
        ps_data.state_info.w = static_cast<float>(iterations);
        ps_data.frame_size.xy = float2(window.size());
        ps_data.start_color = float4(start_color);

        gpu.clear_internal();
        gpu.set_pixel_const_buffer(ps_data);
        gpu.draw_vertex_buffer(screen_mesh);
        gpu.swap_buffers(true);

        // Info
        window.set_title(kl::format(
            "[", std::setw(3), static_cast<int>(1 / timer.get_interval()), "] ",
            "(Iterations: ", iterations, ") ",
            "(Zoom: ", std::fixed, std::setprecision(2), zoom, ") ",
            "(Position: ", position, ")"
        ));
    }
}

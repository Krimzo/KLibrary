#include "examples.h"


static int ITERATIONS = 64;
static float ZOOM = 1.0f;
static kl::Float2 POSITION = { -0.5f, 0.0f };
static kl::Color START_COLOR = kl::colors::ORANGE;

static void input(const kl::Window& window, const float delta_time)
{
    // Keyboard
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

    // Mouse
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

    // Scroll
    static int last_scroll = window.mouse.scroll();
    const int current_scroll = window.mouse.scroll();
    ITERATIONS += (current_scroll - last_scroll) * (window.keyboard.shift ? 10 : 1);
    ITERATIONS = max(ITERATIONS, 0);
    last_scroll = current_scroll;
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
                kl::Color result_color = {};
                result_color.r = (uint8_t) std::stoi(parts[0]);
                result_color.g = (uint8_t) std::stoi(parts[1]);
                result_color.b = (uint8_t) std::stoi(parts[2]);
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

int examples::mandelbrot_main()
{
    kl::Window window = { "Mandelbrot", { 1600, 900 } };
    kl::GPU gpu = { (HWND) window };
    kl::Timer timer = {};

    window.on_resize.emplace_back([&](const kl::Int2 size)
    {
        if (size.x > 0 && size.y > 0) {
            gpu.resize_internal(size);
            gpu.set_viewport_size(size);
        }
    });
    window.maximize();

    // Start
    const std::string shader_sources = kl::read_file_string("shaders/mandelbrot.hlsl");
    kl::RenderShaders shaders = gpu.create_render_shaders(shader_sources);
    const kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();

    // Console
    std::thread(console_read).detach();

    // Update
    while (window.process(false)) {
        timer.update_delta();

        // Input
        input(window, timer.delta());

        // Render
        gpu.clear_internal();

        class MandelbrotPsCb
        {
        public:
            kl::Float4 state_info;
            kl::Float4 frame_size;
            kl::Float4 start_color;
        };

        MandelbrotPsCb ps_data = {};
        ps_data.state_info = { POSITION, ZOOM, (float) ITERATIONS };
        ps_data.frame_size = { window.size(), {} };
        ps_data.start_color = START_COLOR;
        shaders.pixel_shader.update_cbuffer(ps_data);
        gpu.bind_render_shaders(shaders);

        gpu.draw(screen_mesh);
        gpu.swap_buffers(true);

        // Info
        window.set_title(kl::format(
            "(Iterations: ", ITERATIONS, ") ",
            "(Zoom: ", std::fixed, std::setprecision(2), ZOOM, ") ",
            "(Position: ", POSITION, ")"
        ));
    }
    return 0;
}

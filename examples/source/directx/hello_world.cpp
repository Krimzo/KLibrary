#include "klib.h"


static const std::string shader_source = R"(
// Vertex shader
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1);
}

// Pixel shader
float4 p_shader(const float4 screen_position : SV_Position) : SV_Target
{
    return 1;
}
)";

int main()
{
    // Window setup
    kl::BOUND_WINDOW = kl::window::make({ 1600, 900 }, "Hello World");
    auto window = kl::BOUND_WINDOW;

    // GPU setup
    kl::BOUND_GPU = kl::gpu::make(*kl::BOUND_WINDOW);
    auto gpu = kl::BOUND_GPU;

    // Window resize setup
    window->on_resize.push_back([&](const kl::int2& new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu->resize_internal(new_size);
            gpu->set_viewport(new_size);
        }
    });
    window->maximize();

    // Needed raster setup
    auto raster_state = kl::gpu_raster_state::make(false, false, true);
    raster_state->bind();

    // Mesh setup
    auto triangle = kl::gpu_mesh::make({
        { {  0.0f,  0.5f, 0.5f } },
        { { -0.5f, -0.5f, 0.5f } },
        { {  0.5f, -0.5f, 0.5f } },
    });

    // Shader setup
    auto shaders = kl::gpu_shaders::make(shader_source);
    shaders->bind();

    // Loop
    while (window->process(false)) {
        // CDS (Clear-Draw-Swap)
        gpu->clear_internal();
        triangle->draw();
        gpu->swap_buffers(true);
    }
}

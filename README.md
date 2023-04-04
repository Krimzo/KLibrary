## About
General purpose library focused on 3D graphics.  
Goal is to *simplify* use of C++, Win32 and DirectX.  

## D3D11 Triangle Hello World
```cpp
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
    return float4(1, 0.75, 0.35, 1);
}
)";

int main()
{
    // Window setup
    kl::window window = { { 1600, 900 }, "Hello World" };
    kl::gpu gpu = { (HWND) window };

    // Window resize setup
    window.on_resize.push_back([&](const kl::int2& new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
        }
    });
    window.maximize();

    // Fullscreen setup
    window.keyboard.f11.on_press.push_back([&]
    {
        const bool new_state = !window.in_fullscreen();
        window.set_fullscreen(new_state);
        gpu.set_fullscreen(new_state);
    });

    // Mesh setup
    auto triangle = gpu.create_mesh({
        { {  0.0f,  0.5f, 0.5f } },
        { { -0.5f, -0.5f, 0.5f } },
        { {  0.5f, -0.5f, 0.5f } },
    });

    // Shader setup
    auto shaders = gpu.create_render_shaders(shader_source);
    gpu.bind_render_shaders(shaders);

    // CDS (Clear-Draw-Swap)
    while (window.process(false)) {
        gpu.clear_internal(kl::colors::gray);
        gpu.draw_mesh(triangle);
        gpu.swap_buffers(true);
    }
}

```

## Win32 Examples
### Visualising sorting algorithms  
![](examples/screens/visual_sort.png)

### Visualising times table  
![](examples/screens/times_table.png)

## DirectX Examples
### Usage of geometry shaders  
![](examples/screens/geometry_shaders.png)

### Rendering the Mandelbrot set  
![](examples/screens/mandelbrot.png)

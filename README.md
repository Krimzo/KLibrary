## About
General purpose library focused on 3D graphics.  
Goal is to *simplify* use of C++, Win32 and DirectX.  

## D3D11 Triangle Hello World
```cpp
#include "klibrary.h"

static constexpr auto SHADER_SOURCE =
R"(
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(const float4 screen_position : SV_Position) : SV_Target
{
    return float4(1.0f, 0.75f, 0.35f, 1.0f);
}
)";

int main()
{
    kl::Window window{ "Hello World! (D3D11)", { 1600, 900 } };
    kl::GPU gpu{ HWND(window), kl::IS_DEBUG };

    window.on_resize.emplace_back([&](const kl::Int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
        }
    });

    const kl::dx::Buffer triangle = gpu.create_vertex_buffer({
        { {  0.0f,  0.5f, 0.5f } },
        { {  0.5f, -0.5f, 0.5f } },
        { { -0.5f, -0.5f, 0.5f } },
    });
    
    const kl::RenderShaders shaders = gpu.create_render_shaders(SHADER_SOURCE);
    gpu.bind_render_shaders(shaders);

    while (window.process()) {
        gpu.clear_internal(kl::Color{ 30, 30, 30 });
        gpu.draw(triangle);
        gpu.swap_buffers(true);
    }
    return 0;
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

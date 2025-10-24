## About
General purpose Win/C++ library.  
Goal is to *simplify* use of C++, Win32 and DirectX.

## Features
- Graphics & GPU: Abstractions for DirectX 11/DirectX 12 and shader management.  
- Math: 3D mathematics including vectors, matrices, triangles and raytracing primitives.  
- Media: Support for handling audio, images, and video.  
- Utility: Modules for asynchronous operations, data formatting, hashing, and memory safety.  
- Window Management: Simplifies window creation, input handling, and system hooks.  
- Web & Data: Includes functionality for HTTP, sockets, and JSON parsing.  

## Hello Triangle
```cpp
#include "klibrary.h"

static constexpr auto SHADER_SOURCE =
R"(
float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    return float4(1.0f, 0.75f, 0.35f, 1.0f);
}
)";

int main()
{
    kl::Window window{ "Hello World! (D3D11)" };
    kl::GPU gpu{ window.ptr() };

    window.on_resize.emplace_back([&](kl::Int2 size)
    {
        gpu.resize_internal(size);
        gpu.set_viewport_size(size);
    });

    kl::dx::Buffer triangle = gpu.create_vertex_buffer({
        { {  0.0f,  0.5f, 0.5f } },
        { {  0.5f, -0.5f, 0.5f } },
        { { -0.5f, -0.5f, 0.5f } },
    });
    
    kl::Shaders shaders = gpu.create_shaders(SHADER_SOURCE);
    gpu.bind_shaders(shaders);

    while (window.process()) {
        gpu.clear_internal(kl::RGB{ 30, 30, 30 });
        gpu.draw(triangle);
        gpu.swap_buffers(true);
    }
    return 0;
}
```

## Examples
### Visualising sorting algorithms  
![](examples/screens/visual_sort.png)

### Visualising times table  
![](examples/screens/times_table.png)

### Usage of geometry shaders  
![](examples/screens/geometry_shaders.png)

### Rendering the Mandelbrot set  
![](examples/screens/mandelbrot.png)

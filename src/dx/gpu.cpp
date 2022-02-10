#include "KrimzLib/dx/gpu.h"

#include "KrimzLib/console.h"


// Constructor
kl::gpu::gpu(HWND hwnd, int msaa) {
    // Saving MSAA
    this->msaa = msaa;

    // Swapchain info
    DXGI_SWAP_CHAIN_DESC chaindes = {};
    chaindes.BufferCount = 1;                                 // One back buffer
    chaindes.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 32-bit color
    chaindes.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // Usage
    chaindes.OutputWindow = hwnd;                             // Window
    chaindes.SampleDesc.Count = msaa;                         // MSAA
    chaindes.Windowed = true;                                 // Windowed/fullscreen

    // Dev, devcon and chain creation
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        NULL,
        nullptr,
        NULL,
        D3D11_SDK_VERSION,
        &chaindes,
        &chain,
        &dev,
        nullptr,
        &devcon
    );

    // Error checking
    kl::console::error(!dev, "DirectX: Could not create device!");
    kl::console::error(!devcon, "DirectX: Could not create device context!");
    kl::console::error(!chain, "DirectX: Could not create swapchain!");
}

// Destructor
kl::gpu::~gpu() {
    chain->Release();
    devcon->Release();
    dev->Release();
}

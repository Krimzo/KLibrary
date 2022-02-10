#include "KrimzLib/dx/gpu.h"

#include "KrimzLib/console.h"


// Constructor
kl::gpu::gpu(HWND hwnd, int msaa) {
    // Saving MSAA
    this->msaa = msaa;

    // Getting the window size
    RECT clientArea = {};
    GetClientRect(hwnd, &clientArea);

    // Swapchain info
    DXGI_SWAP_CHAIN_DESC chaindes = {};
    chaindes.BufferCount = 1;                                 // One back buffer
    chaindes.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 32-bit color
    chaindes.BufferDesc.Width = clientArea.right;             // Setting the backbuffer width
    chaindes.BufferDesc.Height = clientArea.bottom;           // Setting the backbuffer height
    chaindes.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // Usage
    chaindes.OutputWindow = hwnd;                             // Window
    chaindes.SampleDesc.Count = msaa;                         // MSAA
    chaindes.Windowed = true;                                 // Windowed/fullscreen
    chaindes.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // Allowing fullscreen switching

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
    kl::console::error(!dev, "DirectX: Could not create device!");
    kl::console::error(!devcon, "DirectX: Could not create device context!");
    kl::console::error(!chain, "DirectX: Could not create swapchain!");

    // Getting the back buffer address
    ID3D11Texture2D* buffAddrs = nullptr;
    chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffAddrs);
    kl::console::error(!buffAddrs, "DirectX: Could not get back buffer address!");

    // Creating the render target
    dev->CreateRenderTargetView(buffAddrs, nullptr, &backBuff);
    buffAddrs->Release();

    // Setting the render target
    devcon->OMSetRenderTargets(1, &backBuff, nullptr);

    // Viewport setup
    this->setViewport(kl::ivec2(clientArea.left, clientArea.top), kl::ivec2(clientArea.right, clientArea.bottom));
}

// Destructor
kl::gpu::~gpu() {
    // Exiting fullscreen
    chain->SetFullscreenState(false, nullptr);

    // Memory release
    backBuff->Release();
    chain->Release();
    devcon->Release();
    dev->Release();
}

// Sets the viewport
void kl::gpu::setViewport(const kl::ivec2& pos, const kl::ivec2& size) {
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = float(pos.x);
    viewport.TopLeftY = float(pos.y);
    viewport.Width = float(size.x);
    viewport.Height = float(size.y);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    devcon->RSSetViewports(1, &viewport);
}

// Clears the buffer
void kl::gpu::clear(const kl::vec4& color) {
    devcon->ClearRenderTargetView(backBuff, (float*)&color);
}

// Swaps the buffers
void kl::gpu::swap(bool vSync) {
    chain->Present(vSync, NULL);
}

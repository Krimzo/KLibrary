#include "KrimzLib/dx/gpu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "ImGui/imgui_impl_dx11.h"

#include "KrimzLib/geometry/vertex.h"
#include "KrimzLib/convert.h"


// Constructor
kl::gpu::gpu(HWND hwnd, bool imgui) {
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
    chaindes.SampleDesc.Count = 1;                            // MSAA
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
    if (!dev) {
        std::cout << "DirectX: Could not create device!";
        std::cin.get();
        exit(69);
    }
    if (!devcon) {
        std::cout << "DirectX: Could not create device context!";
        std::cin.get();
        exit(69);
    }
    if (!chain) {
        std::cout << "DirectX: Could not create swapchain!";
        std::cin.get();
        exit(69);
    }

    // Generating the buffers
    this->regenBuffers(kl::ivec2(clientArea.right, clientArea.bottom));

    // Viewport setup
    this->setViewport(kl::ivec2(clientArea.left, clientArea.top), kl::ivec2(clientArea.right, clientArea.bottom));

    // Setting the triangle as the main primitive type
    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ImGui
    this->usingImGui = imgui;
    if (imgui) {
        ImGui_ImplDX11_Init(dev, devcon);
    }
}

// Destructor
kl::gpu::~gpu() {
    // Exiting fullscreen
    chain->SetFullscreenState(false, nullptr);

    // ImGui
    if (usingImGui) {
        ImGui_ImplDX11_Shutdown();
    }

    // Utility cleanup
    samplers.clear();
    textures.clear();
    meshes.clear();
    cbuffers.clear();
    shaders.clear();
    rasters.clear();
    sbuffers.clear();
    
    // Internal cleanup
    delete indexBuff;
    delete depthBuff;
    delete frameBuff;

    // Chain cleanup
    chain->Release();
    devcon->Release();
    dev->Release();
}

// Getters
ID3D11Device* kl::gpu::getDev() {
    return dev;
}
ID3D11DeviceContext* kl::gpu::getCon() {
    return devcon;
}

// Resizes the buffers
void kl::gpu::regenBuffers(const kl::ivec2& size) {
    // Cleanup
    devcon->OMSetRenderTargets(0, nullptr, nullptr);
    if (frameBuff) delete frameBuff;
    if (depthBuff) delete depthBuff;
    if (indexBuff) delete indexBuff;
    chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    // New buffer creation
    frameBuff = new kl::fbuffer(chain, dev, devcon, size.x, size.y);
    depthBuff = new kl::dbuffer(dev, devcon, size.x, size.y);
    indexBuff = new kl::ibuffer(dev, devcon, size.x, size.y);

    // Buffer binding
    this->bindInternal();
    this->setDSState(kl::dbuffer::Default);
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

// Binds the internal render targets
void kl::gpu::bindInternal() {
    ID3D11RenderTargetView* tempBuff[2] = { frameBuff->getView(), indexBuff->getView() };
    devcon->OMSetRenderTargets(2, tempBuff, depthBuff->getView());
}

// Clears the buffer
void kl::gpu::clearColor(const kl::vec4& color) {
    frameBuff->clear(color);
}
void kl::gpu::clearDepth() {
    depthBuff->clear();
}
void kl::gpu::clearIndex() {
    indexBuff->clear();
}
void kl::gpu::clear(const kl::vec4& color) {
    frameBuff->clear(color);
    depthBuff->clear();
    indexBuff->clear();
}

// Swaps the buffers
void kl::gpu::swap(bool vSync) {
    chain->Present(vSync, NULL);
}

// Sets the depth/stencil state
void kl::gpu::setDSState(kl::dbuffer::STATE state) {
    depthBuff->setState(state);
}

// Creates a new rasterizer state
kl::raster* kl::gpu::newRaster(bool wireframe, bool cull, bool cullBack) {
    return rasters.newInst(new kl::raster(dev, devcon, wireframe, cull, cullBack));
}
bool kl::gpu::delRaster(kl::raster* ras) {
    return rasters.delInst(ras);
}

// Compiles and returns shaders
kl::shaders* kl::gpu::newShaders(const std::string& filePath, uint32_t vertBuffSize, uint32_t pixlBuffSize) {
    return shaders.newInst(new kl::shaders(dev, devcon, filePath, vertBuffSize, pixlBuffSize));
}
bool kl::gpu::delShaders(kl::shaders* sha) {
    return shaders.delInst(sha);
}

// Creates a new constant buffer
kl::cbuffer* kl::gpu::newCBuffer(int byteSize) {
    return cbuffers.newInst(new kl::cbuffer(dev, devcon, byteSize));
}
bool kl::gpu::delCBuffer(kl::cbuffer* cbuf) {
    return cbuffers.delInst(cbuf);
}

// Creates a new mesh
kl::mesh* kl::gpu::newMesh(const std::vector<kl::vertex>& vertexData) {
    return meshes.newInst(new kl::mesh(dev, devcon, vertexData));
}
kl::mesh* kl::gpu::newMesh(const std::string& filePath, bool flipZ) {
    return meshes.newInst(new kl::mesh(dev, devcon, filePath, flipZ));
}
bool kl::gpu::delMesh(kl::mesh* mes) {
    return meshes.delInst(mes);
}

// Creates a new texture
kl::texture* kl::gpu::newTexture(const kl::image& img) {
    return textures.newInst(new kl::texture(dev, devcon, img));
}
bool kl::gpu::delTexture(kl::texture* tex) {
    return textures.delInst(tex);
}

// Creates a new sampler
kl::sampler* kl::gpu::newSampler(bool linear, bool mirror) {
    return samplers.newInst(new kl::sampler(dev, devcon, linear, mirror));
}
bool kl::gpu::delSampler(kl::sampler* samp) {
    return samplers.delInst(samp);
}

// SBuffer
kl::sbuffer* kl::gpu::newSBuffer(uint32_t size) {
    return sbuffers.newInst(new kl::sbuffer(dev, devcon, size));
}
bool kl::gpu::delSBuffer(kl::sbuffer* sbuff) {
    return sbuffers.delInst(sbuff);
}

// Returns the picking index
int kl::gpu::getIndex(const kl::ivec2& pos) {
    return indexBuff->getIndex(pos);
}

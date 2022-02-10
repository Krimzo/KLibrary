#include "KrimzLib/dx/gpu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <d3dcompiler.h>

#include "KrimzLib/renderer/vertex.h"
#include "KrimzLib/console.h"
#include "KrimzLib/convert.h"


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

// Sets a new rasterizer state
void kl::gpu::setRaster(bool wireframe, bool cull) {
    // Creating a raster state
    D3D11_RASTERIZER_DESC rasterStateDesc = {};
    rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    rasterStateDesc.CullMode = cull ? D3D11_CULL_BACK : D3D11_CULL_NONE;
    rasterStateDesc.FrontCounterClockwise = true;
    rasterStateDesc.MultisampleEnable = true;
    rasterStateDesc.AntialiasedLineEnable = true;
    ID3D11RasterizerState* rasterState = nullptr;
    dev->CreateRasterizerState(&rasterStateDesc, &rasterState);
    kl::console::error(!rasterState, "DirectX: Could not create rater state!");

    // Setting the raster state and cleanup
    devcon->RSSetState(rasterState);
    rasterState->Release();
}

// Clears the buffer
void kl::gpu::clear(const kl::vec4& color) {
    devcon->ClearRenderTargetView(backBuff, (float*)&color);
}

// Swaps the buffers
void kl::gpu::swap(bool vSync) {
    chain->Present(vSync, NULL);
}

// Compiles and returns shaders
kl::shaders* kl::gpu::newShaders(const std::string& filePath) {
    // Shader blobs
    ID3D10Blob* vsBlob = nullptr;
    ID3D10Blob* psBlob = nullptr;

    // Shader pointers
    ID3D11VertexShader* vs = nullptr;
    ID3D11PixelShader* ps = nullptr;

    // File path as wstring
    std::wstring wFilPath = kl::convert::toWString(filePath);

    // Vertex shader compilation
    D3DCompileFromFile(wFilPath.c_str(), nullptr, nullptr, "vShader", "vs_4_0", NULL, NULL, &vsBlob, nullptr);
    kl::console::error(!vsBlob, "DirectX: Could not compile vertex shader!");
    dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vs);

    // Pixel shader compilation
    D3DCompileFromFile(wFilPath.c_str(), nullptr, nullptr, "pShader", "ps_4_0", NULL, NULL, &psBlob, nullptr);
    kl::console::error(!psBlob, "DirectX: Could not compile pixel shader!");
    dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &ps);

    // Input descriptor buffer
    D3D11_INPUT_ELEMENT_DESC inputDescriptor[3] = {
        {  "POS_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {  "TEX_IN", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORM_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    // Setting the buffer layout
    ID3D11InputLayout* bufferLayout = nullptr;
    dev->CreateInputLayout(inputDescriptor, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &bufferLayout);
    kl::console::error(!bufferLayout, "DirectX: Could not create an input layout!");
    devcon->IASetInputLayout(bufferLayout);

    // Shader return
    return new kl::shaders(devcon, vsBlob, psBlob, vs, ps);
}

// Creates a new constant buffer
kl::cbuffer* kl::gpu::newCBuffer(const int byteSize) {
    // Buffer descriptor setting
    D3D11_BUFFER_DESC bufferDescriptor = {};
    bufferDescriptor.ByteWidth = byteSize;
    bufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDescriptor.MiscFlags = NULL;
    bufferDescriptor.StructureByteStride = 0;

    // Buffer creation
    ID3D11Buffer* buff = nullptr;
    dev->CreateBuffer(&bufferDescriptor, nullptr, &buff);
    kl::console::error(!buff, "DirectX: Could not create a constant buffer!");

    // Buffer return
    return new kl::cbuffer(this->devcon, buff, byteSize);
}

// Creates a new mesh
kl::mesh* kl::gpu::newMesh(const std::vector<kl::vertex>& vertexData) {
    // Buffer descriptor
    D3D11_BUFFER_DESC bufferDescriptor = {};
    bufferDescriptor.ByteWidth = UINT(sizeof(kl::vertex) * vertexData.size());
    bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDescriptor.CPUAccessFlags = NULL;
    bufferDescriptor.MiscFlags = NULL;
    bufferDescriptor.StructureByteStride = 0;

    // Buffer data setting
    D3D11_SUBRESOURCE_DATA bufferData = {};
    bufferData.pSysMem = &vertexData[0];
    bufferData.SysMemPitch = 0;
    bufferData.SysMemSlicePitch = 0;

    // Buffer creation
    ID3D11Buffer* buff = nullptr;
    dev->CreateBuffer(&bufferDescriptor, &bufferData, &buff);
    kl::console::error(!buff, "DirectX: Could not create a vertex buffer!");

    // Returning
    return new kl::mesh(devcon, buff, int(vertexData.size()));
}
kl::mesh* kl::gpu::newMesh(const std::string& filePath, bool flipZ) {
    // Temp vertex buffer
    std::vector<kl::vertex> vertexData;

    // Opening the file
    std::fstream fileStream;
    fileStream.open(filePath, std::ios::in);
    kl::console::error(!fileStream.is_open(), "Mesh: Could not open an object file!");

    // Temp load buffers
    std::vector<kl::vec3> xyzBuffer;
    std::vector<kl::vec2> uvBuffer;
    std::vector<kl::vec3> normBuffer;

    // Z flipper
    const int zFlip = flipZ ? -1 : 1;

    // Parsing data
    std::string fileLine;
    while (std::getline(fileStream, fileLine)) {
        // Splitting the string by spaces
        std::vector<std::string> lineParts;
        std::stringstream lineStream(fileLine);
        for (std::string linePart; std::getline(lineStream, linePart, ' ');) {
            lineParts.push_back(linePart);
        }

        // Parsing the data
        if (lineParts[0] == "v") {
            xyzBuffer.push_back(kl::vec3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
        }
        else if (lineParts[0] == "vt") {
            uvBuffer.push_back(kl::vec2(std::stof(lineParts[1]), std::stof(lineParts[2])));
        }
        else if (lineParts[0] == "vn") {
            normBuffer.push_back(kl::vec3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
        }
        else if (lineParts[0] == "f") {
            for (int i = 1; i < 4; i++) {
                // Getting the world, texture and normal indexes
                std::vector<std::string> linePartParts;
                std::stringstream linePartStream(lineParts[i]);
                for (std::string linePartPart; std::getline(linePartStream, linePartPart, '/');) {
                    linePartParts.push_back(linePartPart);
                }

                // Saving the data
                vertexData.push_back(
                    kl::vertex(
                        xyzBuffer[std::stoi(linePartParts[0]) - 1],
                        uvBuffer[std::stoi(linePartParts[1]) - 1],
                        normBuffer[std::stoi(linePartParts[2]) - 1]
                    )
                );
            }
        }
    }

    // Closing the file
    fileStream.close();

    // Creating the mesh
    return this->newMesh(vertexData);
}

// Creates a new texture
kl::texture* kl::gpu::newTexture(const kl::image& img) {
    // Image copy
    kl::image tempImage = img;
    tempImage.flipVertical();

    // Getting the image pixel data pointer
    const kl::color* imagePointer = (kl::color*)tempImage.pointer();

    // Allocating the temp pixel memory
    unsigned int* pixelData = new unsigned int[tempImage.getWidth() * tempImage.getHeight()];
    kl::console::error(!pixelData, "Memory: Could not allocated temp pixel data!");

    // Reading and storing the pixel data
    for (int i = 0; i < tempImage.getWidth() * tempImage.getHeight(); i++) {
        pixelData[i] = 0xFF;
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].b);
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].g);
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].r);
    }

    // Creating the texture resource
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = tempImage.getWidth();
    texDesc.Height = tempImage.getHeight();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = NULL;
    texDesc.MiscFlags = NULL;
    D3D11_SUBRESOURCE_DATA texData = {};
    texData.pSysMem = pixelData;
    texData.SysMemPitch = tempImage.getWidth() * sizeof(unsigned int);
    ID3D11Texture2D* tex = nullptr;
    dev->CreateTexture2D(&texDesc, &texData, &tex);
    kl::console::error(!tex, "DirectX: Could not create a 2D texture!");

    // Deleting temp pixel data
    delete[] pixelData;

    // Creating a resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1;
    viewDesc.Texture2D.MostDetailedMip = 0;
    ID3D11ShaderResourceView* view = nullptr;
    dev->CreateShaderResourceView(tex, &viewDesc, &view);
    kl::console::error(!view, "DirectX: Could not create a 2D texture view!");
    tex->Release();

    // Returing the texture
    return new kl::texture(devcon, view);
}

// Creates a new sampler
kl::sampler* kl::gpu::newSampler() {
    // Sampler creation
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    ID3D11SamplerState* state = nullptr;
    dev->CreateSamplerState(&sampDesc, &state);
    kl::console::error(!state, "DirectX: Could not create a sampler state!");

    // Sampler return
    return new kl::sampler(devcon, state);
}

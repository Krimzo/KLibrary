#include "gpu/gpu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#ifdef KL_USING_IMGUI
#include "imgui_impl_dx11.h"
#endif

#include "geometry/vertex.h"
#include "utility/convert.h"
#include "utility/console.h"


// Constructor
kl::gpu::gpu(HWND hwnd) {
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
		&device,
		nullptr,
		&devcon
	);
	if (!device) {
		kl::console::show();
		std::cout << "DirectX: Could not create device!";
		std::cin.get();
		exit(69);
	}
	if (!devcon) {
		kl::console::show();
		std::cout << "DirectX: Could not create device context!";
		std::cin.get();
		exit(69);
	}
	if (!chain) {
		kl::console::show();
		std::cout << "DirectX: Could not create swapchain!";
		std::cin.get();
		exit(69);
	}

	// Generating the buffers
	regenBuffers(kl::int2(clientArea.right, clientArea.bottom));

	// Creating a default rasterizer
	bind(newRasterState(false, false));

	// Viewport setup
	viewport(kl::int2(clientArea.left, clientArea.top), kl::int2(clientArea.right, clientArea.bottom));

	// Setting the triangle as the main primitive type
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#ifdef KL_USING_IMGUI
	ImGui_ImplDX11_Init(device, devcon);
#endif
}

// Destructor
kl::gpu::~gpu() {
#ifdef KL_USING_IMGUI
	ImGui_ImplDX11_Shutdown();
#endif

	// Exiting fullscreen
	chain->SetFullscreenState(false, nullptr);

	// Child cleanup
	for (auto& ref : children) {
		ref->Release();
	}
	children.clear();

	// Chain cleanup
	chain->Release();
	devcon->Release();
	device->Release();
}

// Getters
ID3D11Device* kl::gpu::dev() {
	return device;
}
ID3D11DeviceContext* kl::gpu::con() {
	return devcon;
}

// Resizes the buffers
void kl::gpu::regenBuffers(const kl::int2& size) {
	// Cleanup
	bindTargets({});
	if (interFrameBuff) {
		destroy(interFrameBuff);
	}
	if (interDepthBuff) {
		destroy(interDepthBuff);
	}
	chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	// Frame buffer creation
	ID3D11Texture2D* bbTex = newTextureBB();
	interFrameBuff = newTargetView(bbTex);
	destroy(bbTex);

	// Depth buffer creation
	D3D11_TEXTURE2D_DESC dsTexDesc = {};
	dsTexDesc.Width = size.x;
	dsTexDesc.Height = size.y;
	dsTexDesc.MipLevels = 1;
	dsTexDesc.ArraySize = 1;
	dsTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsTexDesc.SampleDesc.Count = 1;
	dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
	dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ID3D11Texture2D* depthTex = newTexture(&dsTexDesc);
	interDepthBuff = newDepthView(depthTex);
	destroy(depthTex);

	// Buffer binding
	bindInternal();
}

// Sets the viewport
void kl::gpu::viewport(const kl::int2& pos, const kl::int2& size) {
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
void kl::gpu::bindInternal(const std::vector<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView* depthView) {
	std::vector<ID3D11RenderTargetView*> combinedTargets = { interFrameBuff };
	for (auto& ref : targets) {
		combinedTargets.push_back(ref);
	}
	devcon->OMSetRenderTargets(UINT(combinedTargets.size()), &combinedTargets[0], depthView ? depthView : interDepthBuff);
}

// Binds given render target
void kl::gpu::bindTargets(const std::vector<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView* depthView) {
	devcon->OMSetRenderTargets(UINT(targets.size()), &targets[0], depthView);
}

// Clears the buffer
void kl::gpu::clearColor(const kl::float4& color) {
	clear(interFrameBuff, color);
}
void kl::gpu::clearDepth() {
	clear(interDepthBuff);
}
void kl::gpu::clear(const kl::float4& color) {
	clear(interFrameBuff, color);
	clear(interDepthBuff);
}

// Swaps the buffers
void kl::gpu::swap(bool vSync) {
	chain->Present(vSync, NULL);
}

// Deletes child instance
bool kl::gpu::destroy(IUnknown* child) {
	if (children.contains(child)) {
		child->Release();
		children.erase(child);
		return true;
	}
	return false;
}

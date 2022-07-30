#include "gpu/gpu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "render/vertex.h"
#include "utility/console.h"

#pragma comment (lib, "d3d11.lib")


kl::gpu::gpu(HWND hwnd, bool predefineCBuffers) : m_CBuffersPredefined(predefineCBuffers) {
	RECT clientArea = {};
	GetClientRect(hwnd, &clientArea);

	DXGI_SWAP_CHAIN_DESC chaindes = {};
	chaindes.BufferCount = 1;
	chaindes.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chaindes.BufferDesc.Width = clientArea.right;
	chaindes.BufferDesc.Height = clientArea.bottom;
	chaindes.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chaindes.OutputWindow = hwnd;
	chaindes.SampleDesc.Count = 1;
	chaindes.Windowed = true;
	chaindes.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&chaindes,
		&m_Chain,
		&m_Device,
		nullptr,
		&m_Context
	);
	kl::console::error(!m_Device, "Failed to create device");
	kl::console::error(!m_Context, "Failed to create device context");
	kl::console::error(!m_Chain, "Failed to create swapchain");

	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	regenInternal({ uint(clientArea.right), uint(clientArea.bottom) });
	viewport({ int(clientArea.left), int(clientArea.top) }, { uint(clientArea.right), uint(clientArea.bottom) });
	bind(newRasterState(false, false));

	if (predefineCBuffers) {
		for (uint i = 0; i < KL_CBUFFER_PREDEFINED_SIZE; i++) {
			m_VertexCBuffers[i] = newCBuffer((i + 1) * 16);
			m_PixelCBuffers[i] = newCBuffer((i + 1) * 16);
		}
	}
}

kl::gpu::~gpu() {
	m_Chain->SetFullscreenState(false, nullptr);

	for (auto& ref : m_Children) {
		ref->Release();
	}
	m_Children.clear();
	m_Chain->Release();
	m_Context->Release();
	m_Device->Release();
}

kl::dx::device kl::gpu::dev() {
	return m_Device;
}
kl::dx::context kl::gpu::con() {
	return m_Context;
}

void kl::gpu::regenInternal(const kl::uint2& size) {
	bindTargets({});
	if (m_FrameBuffer) {
		destroy(m_FrameBuffer);
	}
	if (m_DepthBuffer) {
		destroy(m_DepthBuffer);
	}
	m_Chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	kl::dx::texture bbTex = newTextureBB();
	m_FrameBuffer = newTargetView(bbTex);
	destroy(bbTex);

	kl::dx::desc::texture dsTexDesc = {};
	dsTexDesc.Width = size.x;
	dsTexDesc.Height = size.y;
	dsTexDesc.MipLevels = 1;
	dsTexDesc.ArraySize = 1;
	dsTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsTexDesc.SampleDesc.Count = 1;
	dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
	dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	kl::dx::texture depthTex = newTexture(&dsTexDesc);
	m_DepthBuffer = newDepthView(depthTex);
	destroy(depthTex);

	bindInternal();
}

void kl::gpu::viewport(const kl::uint2& size) {
	viewport({}, size);
}
void kl::gpu::viewport(const kl::int2& pos, const kl::uint2& size) {
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = float(pos.x);
	viewport.TopLeftY = float(pos.y);
	viewport.Width = float(size.x);
	viewport.Height = float(size.y);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_Context->RSSetViewports(1, &viewport);
}

void kl::gpu::bindInternal(const std::vector<kl::dx::view::target> targets, kl::dx::view::depth depthView) {
	std::vector<kl::dx::view::target> combinedTargets = { m_FrameBuffer };
	for (auto& target : targets) {
		combinedTargets.push_back(target);
	}
	m_Context->OMSetRenderTargets(uint(combinedTargets.size()), &combinedTargets[0], depthView ? depthView : m_DepthBuffer);
}

void kl::gpu::bindTargets(const std::vector<kl::dx::view::target> targets, kl::dx::view::depth depthView) {
	m_Context->OMSetRenderTargets(uint(targets.size()), targets.data(), depthView ? depthView : m_DepthBuffer);
}

void kl::gpu::clearColor(const kl::float4& color) {
	clear(m_FrameBuffer, color);
}
void kl::gpu::clearDepth() {
	clear(m_DepthBuffer);
}
void kl::gpu::clear(const kl::float4& color) {
	clear(m_FrameBuffer, color);
	clear(m_DepthBuffer);
}

void kl::gpu::swap(bool vSync) {
	m_Chain->Present(vSync, NULL);
}

bool kl::gpu::destroy(IUnknown* child) {
	if (m_Children.contains(child)) {
		child->Release();
		m_Children.erase(child);
		return true;
	}
	return false;
}

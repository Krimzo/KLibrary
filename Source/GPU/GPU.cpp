#include "gpu/gpu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "Render/Vertex.h"
#include "Utility/Console.h"

#pragma comment (lib, "d3d11.lib")


kl::GPU::GPU(HWND window) {
	RECT windowClientArea = {};
	GetClientRect(window, &windowClientArea);

	DXGI_SWAP_CHAIN_DESC chaindes = {};
	chaindes.BufferCount = 1;
	chaindes.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chaindes.BufferDesc.Width = windowClientArea.right;
	chaindes.BufferDesc.Height = windowClientArea.bottom;
	chaindes.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chaindes.OutputWindow = window;
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
	Assert(!m_Device, "Failed to create device");
	Assert(!m_Context, "Failed to create device context");
	Assert(!m_Chain, "Failed to create swapchain");

	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	resizeInternal({ windowClientArea.right, windowClientArea.bottom });
	setViewport({ windowClientArea.right, windowClientArea.bottom });

	for (int i = 0; i < CBUFFER_PREDEFINED_SIZE; i++) {
		const int bufferSize = (i + 1) * 16;
		m_VertexCBuffers[i] = newCBuffer(bufferSize);
		m_PixelCBuffers[i] = newCBuffer(bufferSize);
		m_ComputeCBuffers[i] = newCBuffer(bufferSize);
	}
}

kl::GPU::~GPU() {
	m_Chain->SetFullscreenState(false, nullptr);

	for (auto& ref : m_Children) {
		ref->Release();
	}
	m_Children.clear();

	m_Chain->Release();
	m_Context->Release();
	m_Device->Release();
}

kl::dx::Device kl::GPU::getDevice() {
	return m_Device;
}

const kl::dx::Device kl::GPU::getDevice() const {
	return m_Device;
}

kl::dx::Context kl::GPU::getContext() {
	return m_Context;
}

const kl::dx::Context kl::GPU::getContext() const {
	return m_Context;
}

void kl::GPU::setViewport(const UInt2& size) {
	setViewport({}, size);
}

void kl::GPU::setViewport(const Int2& position, const UInt2& size) {
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = float(position.x);
	viewport.TopLeftY = float(position.y);
	viewport.Width = float(size.x);
	viewport.Height = float(size.y);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_Context->RSSetViewports(1, &viewport);
}

void kl::GPU::unbindAllTargets() {
	m_Context->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::GPU::bindInternalTargets() {
	m_Context->OMSetRenderTargets(1, &m_FrameBuffer, m_DepthBuffer);
}

void kl::GPU::bindTargets(const Vector<dx::TargetView>& targets, dx::DepthView depthView) {
	m_Context->OMSetRenderTargets(uint(targets.size()), targets.data(), depthView ? depthView : m_DepthBuffer);
}

void kl::GPU::bindTargetsWithInternal(const Vector<dx::TargetView>& additionalTargets, dx::DepthView depthView) {
	Vector<dx::TargetView> combinedTargets = { m_FrameBuffer };
	combinedTargets.insert(combinedTargets.end(), additionalTargets.begin(), additionalTargets.end());
	m_Context->OMSetRenderTargets(uint(combinedTargets.size()), combinedTargets.data(), depthView ? depthView : m_DepthBuffer);
}

void kl::GPU::resizeInternal(const UInt2& size) {
	unbindAllTargets();

	if (m_FrameBuffer) {
		destroy(m_FrameBuffer);
	}
	if (m_DepthBuffer) {
		destroy(m_DepthBuffer);
	}

	m_Chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, NULL);

	dx::Texture backbufferTexture = getBackBuffer();
	m_FrameBuffer = newTargetView(backbufferTexture);
	destroy(backbufferTexture);

	dx::TextureDesc descriptor = {};
	descriptor.Width = size.x;
	descriptor.Height = size.y;
	descriptor.MipLevels = 1;
	descriptor.ArraySize = 1;
	descriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descriptor.SampleDesc.Count = 1;
	descriptor.Usage = D3D11_USAGE_DEFAULT;
	descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	dx::Texture depthTexture = newTexture(&descriptor);
	m_DepthBuffer = newDepthView(depthTexture);
	destroy(depthTexture);

	bindInternalTargets();
}

void kl::GPU::clearInternalColor(const Float4& color) {
	clearTargetView(m_FrameBuffer, color);
}

void kl::GPU::clearInternalDepth(float value) {
	clearDepthView(m_DepthBuffer, value);
}

void kl::GPU::clearInternal() {
	clearInternalColor(Colors::Gray);
	clearInternalDepth(1.0f);
}

void kl::GPU::swapBuffers(bool vSync) {
	m_Chain->Present(vSync, NULL);
}

void kl::GPU::destroy(IUnknown* child) {
	if (m_Children.contains(child)) {
		m_Children.erase(child);
		child->Release();
	}
}

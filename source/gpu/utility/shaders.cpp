#include "gpu/gpu.h"

#include <d3dcompiler.h>

#include "utility/console.h"

#pragma comment (lib, "d3dcompiler.lib")


kl::dx::shader::vertex kl::gpu::newVertexShader(const std::string& source, kl::dx::shader::input* outLayout, const std::vector<kl::dx::shader::desc::input>& desc) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vShader", "vs_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*) blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile vertex shader, Message: " + errorMessage);

	kl::dx::shader::vertex vertShader = nullptr;
	m_Device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &vertShader);
	kl::console::error(!vertShader, "Failed to create vertex shader");

	if (outLayout) {
		kl::dx::shader::desc::input defaulDesc[3] = {
			{ "POS_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEX_IN", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORM_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		m_Device->CreateInputLayout(desc.size() > 0 ? &desc[0] : defaulDesc, desc.size() > 0 ? uint(desc.size()) : 3, blobData->GetBufferPointer(), blobData->GetBufferSize(), outLayout);
		kl::console::error(!*outLayout, "Failed to create input layout");

		m_Children.insert(*outLayout);
	}

	blobData->Release();
	m_Children.insert(vertShader);
	return vertShader;
}
kl::dx::shader::pixel kl::gpu::newPixelShader(const std::string& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "pShader", "ps_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*) blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile pixel shader, Message: " + errorMessage);

	kl::dx::shader::pixel pixlShader = nullptr;
	m_Device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &pixlShader);
	kl::console::error(!pixlShader, "Failed to create pixel shader");

	blobData->Release();
	m_Children.insert(pixlShader);
	return pixlShader;
}
kl::dx::shader::geometry kl::gpu::newGeometryShader(const std::string& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "gShader", "gs_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*) blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile geometry shader, Message: " + errorMessage);

	kl::dx::shader::geometry geomShader = nullptr;
	m_Device->CreateGeometryShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &geomShader);
	kl::console::error(!geomShader, "Failed to create geometry shader");

	blobData->Release();
	m_Children.insert(geomShader);
	return geomShader;
}
kl::dx::shader::compute kl::gpu::newComputeShader(const std::string& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "cShader", "cs_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*) blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile compute shader, Message: " + errorMessage);

	kl::dx::shader::compute compShader = nullptr;
	m_Device->CreateComputeShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &compShader);
	kl::console::error(!compShader, "Failed to create compute shader");

	blobData->Release();
	m_Children.insert(compShader);
	return compShader;
}
kl::shaders kl::gpu::newShaders(const std::string& vertSrc, const std::string& pixlSrc, const std::vector<kl::dx::shader::desc::input>& desc) {
	kl::dx::shader::input inLayout = nullptr;
	kl::dx::shader::vertex vertShader = newVertexShader(vertSrc, &inLayout, desc);
	return kl::shaders(vertShader, newPixelShader(pixlSrc), inLayout);
}
kl::shaders kl::gpu::newShaders(const std::string& fullSrc, const std::vector<kl::dx::shader::desc::input>& desc) {
	return kl::gpu::newShaders(fullSrc, fullSrc, desc);
}

void kl::gpu::bind(kl::dx::shader::vertex sha) {
	m_Context->VSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(kl::dx::shader::pixel sha) {
	m_Context->PSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(kl::dx::shader::geometry sha) {
	m_Context->GSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(kl::dx::shader::compute sha) {
	m_Context->CSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(kl::dx::shader::input layout) {
	m_Context->IASetInputLayout(layout);
}
void kl::gpu::bind(const kl::shaders& shaders, bool bindLayout) {
	bind(shaders.vertexS);
	bind(shaders.pixelS);
	if (bindLayout) bind(shaders.inLayout);
}

void kl::gpu::dispatch(const kl::uint3& size) {
	m_Context->Dispatch(size.x, size.y, size.z);
}
void kl::gpu::execute(kl::dx::shader::compute sha, const kl::uint3& size) {
	bind(sha);
	dispatch(size);
}

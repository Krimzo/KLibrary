#include "gpu/gpu.h"

#include <d3dcompiler.h>

#include "utility/console.h"

#pragma comment (lib, "d3dcompiler.lib")


ID3D11VertexShader* kl::gpu::newVertexShader(const std::string& source, ID3D11InputLayout** outLayout, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vShader", "vs_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile vertex shader, Message: " + errorMessage);

	ID3D11VertexShader* vertShader = nullptr;
	m_Device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &vertShader);
	kl::console::error(!vertShader, "Failed to create vertex shader");

	if (outLayout) {
		D3D11_INPUT_ELEMENT_DESC defaulDesc[3] = {
			{  "POS_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{  "TEX_IN", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORM_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		m_Device->CreateInputLayout(desc.size() > 0 ? &desc[0] : defaulDesc, desc.size() > 0 ? UINT(desc.size()) : 3, blobData->GetBufferPointer(), blobData->GetBufferSize(), outLayout);
		kl::console::error(!*outLayout, "Failed to create input layout");

		m_Children.insert(*outLayout);
	}

	blobData->Release();
	m_Children.insert(vertShader);
	return vertShader;
}
ID3D11PixelShader* kl::gpu::newPixelShader(const std::string& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "pShader", "ps_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile pixel shader, Message: " + errorMessage);

	ID3D11PixelShader* pixlShader = nullptr;
	m_Device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &pixlShader);
	kl::console::error(!pixlShader, "Failed to create pixel shader");

	blobData->Release();
	m_Children.insert(pixlShader);
	return pixlShader;
}
ID3D11GeometryShader* kl::gpu::newGeometryShader(const std::string& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "gShader", "gs_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile geometry shader, Message: " + errorMessage);

	ID3D11GeometryShader* geomShader = nullptr;
	m_Device->CreateGeometryShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &geomShader);
	kl::console::error(!geomShader, "Failed to create geometry shader");

	blobData->Release();
	m_Children.insert(geomShader);
	return geomShader;
}
ID3D11ComputeShader* kl::gpu::newComputeShader(const std::string& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "cShader", "cs_5_0", NULL, NULL, &blobData, &blobError);
	std::string errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	kl::console::error(!blobData, "Failed to compile compute shader, Message: " + errorMessage);

	ID3D11ComputeShader* compShader = nullptr;
	m_Device->CreateComputeShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &compShader);
	kl::console::error(!compShader, "Failed to create compute shader");

	blobData->Release();
	m_Children.insert(compShader);
	return compShader;
}
kl::shaders kl::gpu::newShaders(const std::string& vertSrc, const std::string& pixlSrc, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc) {
	ID3D11InputLayout* inLayout = nullptr;
	ID3D11VertexShader* vertShader = newVertexShader(vertSrc, &inLayout, desc);
	return kl::shaders(vertShader, newPixelShader(pixlSrc), inLayout);
}

void kl::gpu::bind(ID3D11VertexShader* sha) {
	m_Context->VSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(ID3D11PixelShader* sha) {
	m_Context->PSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(ID3D11GeometryShader* sha) {
	m_Context->GSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(ID3D11ComputeShader* sha) {
	m_Context->CSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(ID3D11InputLayout* layout) {
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
void kl::gpu::execute(ID3D11ComputeShader* sha, const kl::uint3& size) {
	bind(sha);
	dispatch(size);
}

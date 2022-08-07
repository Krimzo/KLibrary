#include "GPU/GPU.h"

#include <d3dcompiler.h>

#include "Utility/Console.h"

#pragma comment (lib, "d3dcompiler.lib")


kl::dx::VertexShader kl::GPU::newVertexShader(const String& source, dx::Layout* outLayout, const Vector<dx::LayoutDesc>& descriptors) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vShader", "vs_5_0", NULL, NULL, &blobData, &blobError);
	String errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	Assert(!blobData, "Failed to compile vertex shader, Message: " + errorMessage);

	dx::VertexShader shader = nullptr;
	m_Device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &shader);
	Assert(!shader, "Failed to create vertex shader");

	if (outLayout) {
		if (descriptors.size() > 0) {
			m_Device->CreateInputLayout(descriptors.data(), uint(descriptors.size()), blobData->GetBufferPointer(), blobData->GetBufferSize(), outLayout);
		}
		else {
			dx::LayoutDesc defaultLayoutDescriptors[3] = {
				{ "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "KL_Texture", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			m_Device->CreateInputLayout(defaultLayoutDescriptors, 3, blobData->GetBufferPointer(), blobData->GetBufferSize(), outLayout);
		}
		Assert(!*outLayout, "Failed to create input layout");

		m_Children.insert(*outLayout);
	}

	blobData->Release();
	if (blobError) {
		blobError->Release();
	}

	m_Children.insert(shader);

	return shader;
}

kl::dx::PixelShader kl::GPU::newPixelShader(const String& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "pShader", "ps_5_0", NULL, NULL, &blobData, &blobError);
	String errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	Assert(!blobData, "Failed to compile pixel shader, Message: " + errorMessage);

	dx::PixelShader shader = nullptr;
	m_Device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &shader);
	Assert(!shader, "Failed to create pixel shader");

	blobData->Release();
	if (blobError) {
		blobError->Release();
	}

	m_Children.insert(shader);

	return shader;
}

kl::dx::GeometryShader kl::GPU::newGeometryShader(const String& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "gShader", "gs_5_0", NULL, NULL, &blobData, &blobError);
	String errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	Assert(!blobData, "Failed to compile geometry shader, Message: " + errorMessage);

	dx::GeometryShader shader = nullptr;
	m_Device->CreateGeometryShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &shader);
	Assert(!shader, "Failed to create geometry shader");

	blobData->Release();
	if (blobError) {
		blobError->Release();
	}

	m_Children.insert(shader);

	return shader;
}

kl::dx::ComputeShader kl::GPU::newComputeShader(const String& source) {
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "cShader", "cs_5_0", NULL, NULL, &blobData, &blobError);
	String errorMessage = blobError ? (char*)blobError->GetBufferPointer() : "Unknown";
	Assert(!blobData, "Failed to compile compute shader, Message: " + errorMessage);

	dx::ComputeShader shader = nullptr;
	m_Device->CreateComputeShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &shader);
	Assert(!shader, "Failed to create compute shader");

	blobData->Release();
	if (blobError) {
		blobError->Release();
	}

	m_Children.insert(shader);

	return shader;
}

kl::Shaders kl::GPU::newShaders(const String& vertexSource, const String& pixelSource, const Vector<dx::LayoutDesc>& descriptors) {
	dx::Layout layout = nullptr;
	dx::VertexShader vertexShader = newVertexShader(vertexSource, &layout, descriptors);
	return Shaders(vertexShader, newPixelShader(pixelSource), layout);
}

kl::Shaders kl::GPU::newShaders(const String& source, const Vector<dx::LayoutDesc>& descriptors) {
	return newShaders(source, source, descriptors);
}

void kl::GPU::bindVertexShader(dx::VertexShader shader) {
	m_Context->VSSetShader(shader, nullptr, 0);
}

void kl::GPU::bindPixelShader(dx::PixelShader shader) {
	m_Context->PSSetShader(shader, nullptr, 0);
}

void kl::GPU::bindGeometryShader(dx::GeometryShader shader) {
	m_Context->GSSetShader(shader, nullptr, 0);
}

void kl::GPU::bindComputeShader(dx::ComputeShader shader) {
	m_Context->CSSetShader(shader, nullptr, 0);
}

void kl::GPU::bindLayout(dx::Layout layout) {
	m_Context->IASetInputLayout(layout);
}

void kl::GPU::bindShaders(const Shaders& shaders) {
	bindVertexShader(shaders.vertexShader);
	bindPixelShader(shaders.pixelShader);
	bindLayout(shaders.layout);
}

void kl::GPU::dispatchComputeShader(const UInt3& size) {
	m_Context->Dispatch(size.x, size.y, size.z);
}

void kl::GPU::executeComputeShader(dx::ComputeShader shader, const UInt3& size) {
	bindComputeShader(shader);
	dispatchComputeShader(size);
}

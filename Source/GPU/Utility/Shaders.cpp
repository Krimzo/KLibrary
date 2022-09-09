#include "GPU/GPU.h"

#include <d3dcompiler.h>

#include "Utility/Console.h"

#pragma comment (lib, "d3dcompiler.lib")


struct Blobs {
	ID3DBlob* data = nullptr;
	ID3DBlob* error = nullptr;

	Blobs() {}
	Blobs(const Blobs&) = delete;
	void operator=(const Blobs&) = delete;
	~Blobs() {
		if (data) data->Release();
		if (error) error->Release();
	}

	operator bool() const {
		return bool(data);
	}

	void* dataPointer() {
		if (data) {
			return data->GetBufferPointer();
		}
		return nullptr;
	}

	kl::uint64 dataSize() {
		if (data) {
			return data->GetBufferSize();
		}
		return 0;
	}

	kl::String getError() {
		if (error) {
			return (char*)error->GetBufferPointer();
		}
		return "Unknown";
	}
};

kl::dx::VertexShader kl::GPU::newVertexShader(const String& source, dx::Layout* outLayout, const Vector<dx::LayoutDesc>& descriptors) {
	Blobs blobs = {};

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vShader", "vs_5_0", NULL, NULL, &blobs.data, &blobs.error);
	if (Warning(!blobs, "Failed to compile vertex shader. Error: " + blobs.getError())) {
		return nullptr;
	}

	dx::VertexShader shader = nullptr;
	m_Device->CreateVertexShader(blobs.dataPointer(), blobs.dataSize(), NULL, &shader);
	if (Warning(!shader, "Failed to create vertex shader")) {
		return nullptr;
	}

	if (outLayout) {
		if (descriptors.size() > 0) {
			m_Device->CreateInputLayout(descriptors.data(), uint(descriptors.size()), blobs.dataPointer(), blobs.dataSize(), outLayout);
		}
		else {
			dx::LayoutDesc defaultLayoutDescriptors[3] = {
				{ "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "KL_Texture", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			m_Device->CreateInputLayout(defaultLayoutDescriptors, 3, blobs.dataPointer(), blobs.dataSize(), outLayout);
		}
		if (!Warning(!*outLayout, "Failed to create input layout")) {
			m_Children.insert(*outLayout);
		}
	}

	m_Children.insert(shader);

	return shader;
}

kl::dx::PixelShader kl::GPU::newPixelShader(const String& source) {
	Blobs blobs = {};

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "pShader", "ps_5_0", NULL, NULL, &blobs.data, &blobs.error);
	if (Warning(!blobs, "Failed to compile pixel shader. Error: " + blobs.getError())) {
		return nullptr;
	}

	dx::PixelShader shader = nullptr;
	m_Device->CreatePixelShader(blobs.dataPointer(), blobs.dataSize(), NULL, &shader);
	if (Warning(!shader, "Failed to create pixel shader")) {
		return nullptr;
	}

	m_Children.insert(shader);

	return shader;
}

kl::dx::GeometryShader kl::GPU::newGeometryShader(const String& source) {
	Blobs blobs = {};

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "gShader", "gs_5_0", NULL, NULL, &blobs.data, &blobs.error);
	if (Warning(!blobs, "Failed to compile geometry shader. Error: " + blobs.getError())) {
		return nullptr;
	}

	dx::GeometryShader shader = nullptr;
	m_Device->CreateGeometryShader(blobs.dataPointer(), blobs.dataSize(), NULL, &shader);
	if (Warning(!shader, "Failed to create geometry shader")) {
		return nullptr;
	}

	m_Children.insert(shader);

	return shader;
}

kl::dx::ComputeShader kl::GPU::newComputeShader(const String& source) {
	Blobs blobs = {};

	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "cShader", "cs_5_0", NULL, NULL, &blobs.data, &blobs.error);
	if (Warning(!blobs, "Failed to compile compute shader. Error: " + blobs.getError())) {
		return nullptr;
	}

	dx::ComputeShader shader = nullptr;
	m_Device->CreateComputeShader(blobs.dataPointer(), blobs.dataSize(), NULL, &shader);
	if (Warning(!shader, "Failed to create compute shader")) {
		return nullptr;
	}

	m_Children.insert(shader);

	return shader;
}

kl::Shaders kl::GPU::newShaders(const String& vertexSource, const String& pixelSource, const Vector<dx::LayoutDesc>& descriptors) {
	dx::Layout layout = nullptr;
	dx::VertexShader vertexShader = newVertexShader(vertexSource, &layout, descriptors);
	return { vertexShader, newPixelShader(pixelSource), layout };
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

void kl::GPU::destroy(const Shaders& shaders) {
	destroy(shaders.vertexShader);
	destroy(shaders.pixelShader);
	destroy(shaders.layout);
}

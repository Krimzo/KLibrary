#include "gpu/gpu.h"

#include <d3dcompiler.h>

#include "utility/convert.h"
#include "utility/console.h"

#pragma comment (lib, "d3dcompiler.lib")


// Shader creation
ID3D11VertexShader* kl::gpu::newVertexShader(const std::string& source, ID3D11InputLayout** outLayout, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc) {
	// Blobs
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	// Compilation
	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vShader", "vs_5_0", NULL, NULL, &blobData, &blobError);
	if (!blobData) {
		kl::console::show();
		if (blobError) {
			std::cout << "DirectX: Could not compile vertex shader!\nError: " << (char*)blobError->GetBufferPointer();
		}
		else {
			std::cout << "DirectX: Could not compile vertex shader!\nError: Unknown";
		}
		std::cin.get();
		exit(69);
	}

	// Shader creation
	ID3D11VertexShader* vertShader = nullptr;
	device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &vertShader);
	if (!vertShader) {
		kl::console::show();
		std::cout << "DirectX: Could not create vertex shader!";
		std::cin.get();
		exit(69);
	}

	// Layout output check
	if (outLayout) {
		// Input descriptor
		D3D11_INPUT_ELEMENT_DESC defaulDesc[3] = {
			{  "POS_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{  "TEX_IN", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORM_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// Creating the input layout
		device->CreateInputLayout(desc.size() > 0 ? &desc[0] : defaulDesc, desc.size() > 0 ? UINT(desc.size()) : 3, blobData->GetBufferPointer(), blobData->GetBufferSize(), outLayout);
		if (!*outLayout) {
			kl::console::show();
			std::cout << "DirectX: Could not create an input layout!";
			std::cin.get();
			exit(69);
		}

		// Saving child
		children.push_back(*outLayout);
	}

	// Cleanup
	blobData->Release();

	// Saving child
	children.push_back(vertShader);

	// Return
	return vertShader;
}
ID3D11PixelShader* kl::gpu::newPixelShader(const std::string& source) {
	// Blobs
	ID3DBlob* blobData = nullptr;
	ID3DBlob* blobError = nullptr;

	// Compilation
	D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "pShader", "ps_5_0", NULL, NULL, &blobData, &blobError);
	if (!blobData) {
		kl::console::show();
		if (blobError) {
			std::cout << "DirectX: Could not compile pixel shader!\nError: " << (char*)blobError->GetBufferPointer();
		}
		else {
			std::cout << "DirectX: Could not compile pixel shader!\nError: Unknown";
		}
		std::cin.get();
		exit(69);
	}

	// Shader creation
	ID3D11PixelShader* pixlShader = nullptr;
	device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &pixlShader);
	if (!pixlShader) {
		kl::console::show();
		std::cout << "DirectX: Could not create pixel shader!";
		std::cin.get();
		exit(69);
	}

	// Cleanup
	blobData->Release();

	// Saving child
	children.push_back(pixlShader);

	// Return
	return pixlShader;
}

// Shader binding
void kl::gpu::bind(ID3D11VertexShader* sha) {
	devcon->VSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(ID3D11PixelShader* sha) {
	devcon->PSSetShader(sha, nullptr, 0);
}
void kl::gpu::bind(ID3D11InputLayout* layout) {
	devcon->IASetInputLayout(layout);
}

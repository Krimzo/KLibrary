#include "gpu/gpu.h"


ID3D11SamplerState* kl::gpu::newSamplerState(D3D11_SAMPLER_DESC* desc)
{
	// Sampler state creation
	ID3D11SamplerState* sampState = nullptr;
	device->CreateSamplerState(desc, &sampState);
	if (!sampState)
	{
		std::cout << "DirectX: Could not create a sampler state!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	children.push_back(sampState);

	// Return
	return sampState;
}

ID3D11SamplerState* kl::gpu::newSamplerState(bool linear, bool mirror)
{
	// Sampler descriptor
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;

	// Return
	return this->newSamplerState(&sampDesc);
}

// Binds the sampler
void kl::gpu::bind(ID3D11SamplerState* sampState, int slot)
{
	devcon->PSSetSamplers(slot, 1, &sampState);
}

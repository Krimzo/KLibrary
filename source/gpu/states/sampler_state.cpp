#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11SamplerState* kl::gpu::newSamplerState(D3D11_SAMPLER_DESC* desc) {
	ID3D11SamplerState* sampState = nullptr;
	m_Device->CreateSamplerState(desc, &sampState);
	kl::console::error(!sampState, "Failed to create sampler state");

	m_Children.insert(sampState);
	return sampState;
}

ID3D11SamplerState* kl::gpu::newSamplerState(bool linear, bool mirror) {
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;

	return newSamplerState(&sampDesc);
}

void kl::gpu::bind(ID3D11SamplerState* sampState, uint slot) {
	m_Context->PSSetSamplers(slot, 1, &sampState);
}

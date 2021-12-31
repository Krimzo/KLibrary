#pragma once


namespace kl {
	namespace dx {
		struct sampler {
			sampler(ID3D11Device* dev) {
				D3D11_SAMPLER_DESC sampDesc = {};
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				dev->CreateSamplerState(&sampDesc, &state);
				kl::console::error(!state, "DirectX: Could not create a sampler state!");
			}
			~sampler() {
				state->Release();
			}

			// Returns a pointer to the sampler state
			ID3D11SamplerState* getState() {
				return state;
			}

		private:
			ID3D11SamplerState* state = nullptr;
		};
	}
}

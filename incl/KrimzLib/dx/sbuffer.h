#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>


namespace kl {
	class sbuffer {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11DepthStencilState* state = nullptr;
		ID3D11DepthStencilView* dview = nullptr;
		ID3D11ShaderResourceView* sview = nullptr;
		ID3D11SamplerState* samp = nullptr;
		uint32_t size = 0;

	public:

		// Constructor
		sbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, uint32_t size);

		// Desctructor
		~sbuffer();

		// Binds shadow states
		void bindTarget();

		// Binds shadows sampler and texture
		void bindTexture(int sampSlot, int texSlot);

		// Sets the proper viewport
		void fixViewport();

		// Returns the size
		uint32_t getSize();

		// Clears the buffer
		void clear();
	};
}

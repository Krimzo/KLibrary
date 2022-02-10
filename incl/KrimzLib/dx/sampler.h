#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/console.h"


namespace kl {
	class sampler {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11SamplerState* state = nullptr;

	public:
		// Constructor
		sampler(ID3D11DeviceContext* devcon, ID3D11SamplerState* state);

		// Destructor
		~sampler();

		// Binds the smapler
		void bind(int slot) const;
	};
}

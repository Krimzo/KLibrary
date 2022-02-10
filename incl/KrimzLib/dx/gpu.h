#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/dx/gpu.h"

#pragma comment (lib, "d3d11.lib")


namespace kl {
	class gpu {
	private:
		// DirectX pointers
		IDXGISwapChain* chain = nullptr;
		ID3D11Device* dev = nullptr;
		ID3D11DeviceContext* devcon = nullptr;

		// MSAA
		int msaa = 0;

	public:
		// Constructor
		gpu(HWND hwnd, int msaa = 2);

		// Destructor
		~gpu();
	};
}

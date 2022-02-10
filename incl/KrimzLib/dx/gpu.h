#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/dx/gpu.h"
#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/vecmath/vec4.h"

#pragma comment (lib, "d3d11.lib")


namespace kl {
	class gpu {
	private:
		// DirectX pointers
		IDXGISwapChain* chain = nullptr;
		ID3D11Device* dev = nullptr;
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11RenderTargetView* backBuff = nullptr;

		// MSAA
		int msaa = 0;

	public:
		// Constructor
		gpu(HWND hwnd, int msaa = 2);

		// Destructor
		~gpu();

		// Sets the viewport
		void setViewport(const kl::ivec2& pos, const kl::ivec2& size);

		// Clears the buffer
		void clear(const kl::vec4& color);

		// Swaps the buffers
		void swap(bool vSync);
	};
}

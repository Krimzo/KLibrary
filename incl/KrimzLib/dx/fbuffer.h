#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/math/float4.h"


namespace kl {
	class fbuffer {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11RenderTargetView* view = nullptr;

	public:
		// Constructor
		fbuffer(IDXGISwapChain* chain, ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height);

		// Desctructor
		~fbuffer();

		// Returns the view pointer
		ID3D11RenderTargetView* getView();

		// Clears the buffer
		void clear(const kl::float4& color);
	};
}

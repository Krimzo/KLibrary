#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/vecmath/vec4.h"


namespace kl {
	class fbuffer {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11RenderTargetView* backBuff = nullptr;
		ID3D11DepthStencilState* dsState_enabled = nullptr;
		ID3D11DepthStencilState* dsState_disabled = nullptr;
		ID3D11DepthStencilView* dsBuff = nullptr;

	public:
		// Constructor
		fbuffer(IDXGISwapChain* chain, ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height, int msaa);

		// Desctructor
		~fbuffer();

		// Binds the buffer
		void bind(bool depthTest);

		// Clears the buffer
		void clearColor(const kl::vec4& color);
		void clearDepth();
	};
}

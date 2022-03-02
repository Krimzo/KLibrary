#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/math/int2.h"


namespace kl {
	class ibuffer {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11RenderTargetView* view = nullptr;
		ID3D11Texture2D* indTex = nullptr;
		ID3D11Texture2D* stagTex = nullptr;
		int width = 0;
		int height = 0;

	public:
		// Constructor
		ibuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height);

		// Desctructor
		~ibuffer();

		// Returns the view pointer
		ID3D11RenderTargetView* getView();

		// Clears the buffer
		void clear();

		// Returns the picking index
		int getIndex(const kl::int2& pos);
	};
}

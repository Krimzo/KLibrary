#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>


namespace kl {
	class raster {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11RasterizerState* state = nullptr;

	public:
		// Constructor
		raster(ID3D11Device* dev, ID3D11DeviceContext* devcon, bool wireframe, bool cull, bool cullBack);

		// Destructor
		~raster();

		// Binds the raster
		void bind() const;
	};
}

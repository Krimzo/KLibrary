#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>


namespace kl {
	class mesh {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11Buffer* buff = nullptr;
		int vertexCount = 0;

	public:
		// Constructors
		mesh(ID3D11DeviceContext* devcon, ID3D11Buffer* buff, int vertexCount);

		// Destructor
		~mesh();

		// Draws the mesh
		void draw() const;
	};
}

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/geometry/vertex.h"


namespace kl {
	class mesh {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11Buffer* buff = nullptr;
		UINT vertexCount = 0;

	public:
		// Constructors
		mesh(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::vector<kl::vertex>& vertexData);
		mesh(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& filePath, bool flipZ);

		// Destructor
		~mesh();

		// Draws the mesh
		void draw() const;
	};
}

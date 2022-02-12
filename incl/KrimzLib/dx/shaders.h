#pragma once

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "KrimzLib/dx/cbuffer.h"

#pragma comment (lib, "d3dcompiler.lib")


namespace kl {
	class shaders {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11VertexShader* vs = nullptr;
		ID3D11PixelShader* ps = nullptr;
		kl::cbuffer* vs_data = nullptr;
		kl::cbuffer* ps_data = nullptr;

	public:
		// Constructors
		shaders(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& filePath, uint32_t vertBuffSize, uint32_t pixlBuffSize);

		// Destructor
		~shaders();

		// Binds the shaders
		void bind() const;

		// Sets the cbuffer data
		void setVertData(void* cbuff);
		void setPixlData(void* cbuff);
	};
}

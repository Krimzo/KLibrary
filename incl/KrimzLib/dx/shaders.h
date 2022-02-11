#pragma once

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib")


namespace kl {
	class shaders {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11VertexShader* vs = nullptr;
		ID3D11PixelShader* ps = nullptr;

	public:
		// Constructors
		shaders(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& filePath);

		// Destructor
		~shaders();

		// Binds the shaders
		void bind() const;
	};
}

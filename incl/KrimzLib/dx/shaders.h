#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>


namespace kl {
	class shaders {
	private:
		ID3D11DeviceContext* devcon;
		ID3D10Blob* vsBlob;
		ID3D10Blob* psBlob;
		ID3D11VertexShader* vs;
		ID3D11PixelShader* ps;

	public:
		// Constructors
		shaders(ID3D11DeviceContext* devcon, ID3D10Blob* vsBlob, ID3D10Blob* psBlob, ID3D11VertexShader* vs, ID3D11PixelShader* ps);

		// Destructor
		~shaders();

		// Binds the shaders
		void bind() const;
	};
}

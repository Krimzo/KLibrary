#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/color/image.h"


namespace kl {
	class texture {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11ShaderResourceView* view = nullptr;

	public:
		// Constructors
		texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const kl::image& img);
		texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);

		// Destructor
		~texture();

		// Binds the texture
		void bind(int slot);
	};
}

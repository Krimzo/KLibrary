#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/graphics/image.h"


namespace kl {
	class texture {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11ShaderResourceView* view = nullptr;

	public:
		// Constructor
		texture(ID3D11DeviceContext* devcon, ID3D11ShaderResourceView* view);

		// Destructor
		~texture();

		// Binds the texture
		void bind(int slot) const;
	};
}

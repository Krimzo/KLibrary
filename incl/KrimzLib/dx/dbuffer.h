#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/vecmath/vec4.h"


namespace kl {
	class dbuffer {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11DepthStencilState* stateDefault = nullptr;
		ID3D11DepthStencilState* stateDisabled = nullptr;
		ID3D11DepthStencilState* stateWrite = nullptr;
		ID3D11DepthStencilState* stateMask = nullptr;
		ID3D11DepthStencilView* view = nullptr;

	public:
		// Constructor
		dbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height);

		// Desctructor
		~dbuffer();

		// Binds the buffer
		enum STATE {
			Default = 0,
			Disabled,
			Write,
			Mask
		};
		void setState(STATE state);

		// Returns the view pointer
		ID3D11DepthStencilView* getView();

		// Clears the buffer
		void clear();
	};
}

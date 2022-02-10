#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/console.h"


namespace kl {
	class cbuffer {
    private:
        ID3D11Buffer* buff = nullptr;
        ID3D11DeviceContext* devcon = nullptr;
        int size = 0;

    public:
		// Constructor
        cbuffer(ID3D11DeviceContext* devcon, ID3D11Buffer* buff, int byteSize);

        // Destructor
        ~cbuffer();

        // Binds the buffer
        void bind(const int slot);

        // Sets the data of the buffer
        void setData(void* data);
	};
}

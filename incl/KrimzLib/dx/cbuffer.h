#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/utility/console.h"


namespace kl {
	class cbuffer {
    private:
        ID3D11DeviceContext* devcon = nullptr;
        ID3D11Buffer* buff = nullptr;
        int size = 0;

    public:
		// Constructor
        cbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, uint32_t byteSize);

        // Destructor
        ~cbuffer();

        // Binds the buffer
        void bind(bool pixlSha, int slot);

        // Sets the data of the buffer
        void setData(void* data);
	};
}

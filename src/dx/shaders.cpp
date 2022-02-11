#include "KrimzLib/dx/shaders.h"

#include "KrimzLib/convert.h"


// Constructors
kl::shaders::shaders(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& filePath) {
    // Saving the devcon
	this->devcon = devcon;

    // Shader blobs
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    // Error blobs
    ID3DBlob* vsError = nullptr;
    ID3DBlob* psError = nullptr;

    // File path as wstring
    const std::wstring wFilPath = kl::convert::toWString(filePath);

    // Vertex shader compilation
    D3DCompileFromFile(wFilPath.c_str(), nullptr, nullptr, "vShader", "vs_4_0", NULL, NULL, &vsBlob, &vsError);
    if (!vsBlob) {
        vsError->Release();
        std::cout << "DirectX: Could not compile vertex shader!\nError: " << (char*)vsError->GetBufferPointer();
        std::cin.get();
        exit(69);
    }
    dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vs);
    if (!vs) {
        std::cout << "DirectX: Could not create vertex shader!";
        std::cin.get();
        exit(69);
    }

    // Pixel shader compilation
    D3DCompileFromFile(wFilPath.c_str(), nullptr, nullptr, "pShader", "ps_4_0", NULL, NULL, &psBlob, &psError);
    if (!vsBlob) {
        psError->Release();
        std::cout << "DirectX: Could not compile pixel shader!\nError: " << (char*)psError->GetBufferPointer();
        std::cin.get();
        exit(69);
    }
    dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &ps);
    if (!ps) {
        std::cout << "DirectX: Could not create pixel shader!";
        std::cin.get();
        exit(69);
    }

    // Defining kl::vertex input descriptor
    D3D11_INPUT_ELEMENT_DESC inputDesc[3] = {
        {  "POS_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {  "TEX_IN", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORM_IN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    // Creating the input layout
    ID3D11InputLayout* inputLayout = nullptr;
    dev->CreateInputLayout(inputDesc, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
    if (!inputLayout) {
        std::cout << "DirectX: Could not create an input layout!";
        std::cin.get();
        exit(69);
    }

    // Setting the input layout
    devcon->IASetInputLayout(inputLayout);
    inputLayout->Release();

    // Cleanup
    vsBlob->Release();
    psBlob->Release();
}

// Destructor
kl::shaders::~shaders() {
	vs->Release();
	ps->Release();
}

// Binds the shaders
void kl::shaders::bind() const {
	devcon->VSSetShader(this->vs, nullptr, 0);
	devcon->PSSetShader(this->ps, nullptr, 0);
}

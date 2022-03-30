#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "gpu/gpu.h"
#include "color/image.h"
#include "math/mat4.h"
#include "geometry/vertex.h"


namespace kl {
	class skybox {
	private:
		// DirectX pointers
		kl::gpu* gpu = nullptr;
		ID3D11VertexShader* sky_vtx = nullptr;
		ID3D11PixelShader* sky_pxl = nullptr;
		ID3D11Buffer* vtx_cb = nullptr;
		ID3D11Buffer* box_mes = nullptr;
		ID3D11ShaderResourceView* box_tex = nullptr;

	public:
		// Name
		std::string name;

		// Constructor/destructor
		skybox(kl::gpu* gpu, const std::string& name, const kl::image& fullbox);
		skybox(kl::gpu* gpu, const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		~skybox();

		// Renders the cubemap
		void render(const kl::mat4& vpMat) const;
	};
}

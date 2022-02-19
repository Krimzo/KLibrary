#pragma once

#include <iostream>
#include <vector>

#include "KrimzLib/dx/gpu.h"
#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/cbuffer.h"
#include "KrimzLib/graphics/image.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/geometry/vertex.h"


namespace kl {
	class skybox {
	private:
		// dx data
		kl::mesh* box_mes = nullptr;
		kl::texture* box_tex = nullptr;
		kl::shaders* sky_sh = nullptr;

		// Box vertices
		static const std::vector<kl::vertex> boxVertices;

	public:
		// Name
		std::string name;

		// Constructor/destructor
		skybox(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& name, const kl::image& fullbox);
		skybox(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		~skybox();

		// Renders the cubemap
		void render(const kl::mat4& vpMat) const;
	};
}

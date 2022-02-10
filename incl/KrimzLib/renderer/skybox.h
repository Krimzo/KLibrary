#pragma once

#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/graphics/image.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/renderer/vertex.h"


namespace kl {
	class skybox {
	private:
		// gl data
		kl::mesh* box_mes = nullptr;
		kl::texture* box_tex = nullptr;
		//kl::glsl* box_sha = nullptr;
		//kl::uniform vp_uni;

		// Box vertices
		static const std::vector<kl::vertex> boxVertices;

		// Skybox shader sources
		static const std::string vertSource;
		static const std::string fragSource;

	public:
		// Constructor/destructor
		skybox(const kl::image& fullbox);
		skybox(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		~skybox();

		// Renders the cubemap
		void render(const kl::mat4& vpMat) const;
	};
}

#pragma once

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/gl/mesh.h"
#include "KrimzLib/gl/texture.h"
#include "KrimzLib/gl/shaders.h"
#include "KrimzLib/graphics/image.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/renderer3D/vertex3D.h"


namespace kl {
	class skybox {
	private:
		// gl data
		kl::mesh* box_mes = nullptr;
		kl::texture* box_tex = nullptr;
		kl::shaders* box_sha = nullptr;
		kl::uniform vp_uni;

		// Box vertices
		static const std::vector<kl::vertex3D> boxVertices;

		// Skybox shader sources
		static const std::string vertSource;
		static const std::string fragSource;

	public:
		// Constructor/destructor
		skybox(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		~skybox();

		// Renders the cubemap
		void render(const kl::mat4& vpMat) const;
	};
}

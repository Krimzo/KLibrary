#include "KrimzLib/renderer3D/skybox.h"


// Skybox vertex shader
const std::string kl::skybox::vertSource = R"(
	#version 330

	// Input arrays
	layout (location = 0) in vec3 world;

	// Transformation matrices
	uniform mat4 vp;

	// Interpolated values output
	out vec3 interTex;

	void main() {
		// Outputting the transformed world coords
		gl_Position = (vp * vec4(world, 0)).xyww;
    
		// Outputting the interpolated texture coords
		interTex = world;
	}
)";

// Skybox fragment shader
const std::string kl::skybox::fragSource = R"(
	#version 330

	// Skybox texture
	uniform samplerCube skybox;

	// Interpolated values input
	in vec3 interTex;

	// Pixel color output
	out vec4 pixelColor;

	void main () {
		// Setting the pixel color
		pixelColor = texture(skybox, interTex);
	}
)";

// Skybox box vertices
const std::vector<kl::vertex3D> kl::skybox::boxVertices = {
	kl::vertex3D(kl::vec3( 1,  1, -1)), kl::vertex3D(kl::vec3( 1, -1, -1)), kl::vertex3D(kl::vec3( 1, -1,  1)),
	kl::vertex3D(kl::vec3( 1, -1,  1)), kl::vertex3D(kl::vec3( 1,  1,  1)), kl::vertex3D(kl::vec3( 1,  1, -1)),
	kl::vertex3D(kl::vec3(-1,  1, -1)), kl::vertex3D(kl::vec3(-1,  1,  1)), kl::vertex3D(kl::vec3(-1, -1,  1)),
	kl::vertex3D(kl::vec3(-1, -1,  1)), kl::vertex3D(kl::vec3(-1, -1, -1)), kl::vertex3D(kl::vec3(-1,  1, -1)),
	kl::vertex3D(kl::vec3( 1,  1, -1)), kl::vertex3D(kl::vec3( 1,  1,  1)), kl::vertex3D(kl::vec3(-1,  1,  1)),
	kl::vertex3D(kl::vec3(-1,  1,  1)), kl::vertex3D(kl::vec3(-1,  1, -1)), kl::vertex3D(kl::vec3( 1,  1, -1)),
	kl::vertex3D(kl::vec3( 1, -1, -1)), kl::vertex3D(kl::vec3(-1, -1, -1)), kl::vertex3D(kl::vec3(-1, -1,  1)),
	kl::vertex3D(kl::vec3(-1, -1,  1)), kl::vertex3D(kl::vec3( 1, -1,  1)), kl::vertex3D(kl::vec3( 1, -1, -1)),
	kl::vertex3D(kl::vec3( 1,  1,  1)), kl::vertex3D(kl::vec3( 1, -1,  1)), kl::vertex3D(kl::vec3(-1, -1,  1)),
	kl::vertex3D(kl::vec3(-1, -1,  1)), kl::vertex3D(kl::vec3(-1,  1,  1)), kl::vertex3D(kl::vec3( 1,  1,  1)),
	kl::vertex3D(kl::vec3( 1,  1, -1)), kl::vertex3D(kl::vec3(-1,  1, -1)), kl::vertex3D(kl::vec3(-1, -1, -1)),
	kl::vertex3D(kl::vec3(-1, -1, -1)), kl::vertex3D(kl::vec3( 1, -1, -1)), kl::vertex3D(kl::vec3( 1,  1, -1))
};

// Constructor/destructor
kl::skybox::skybox(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
	// Generating the box mesh
	box_mes = new kl::mesh(boxVertices);

	// Generating the box texture
	box_tex = new kl::texture(front, back, left, right, top, bottom);

	// Compiling skybox shaders
	box_sha = new kl::shaders(vertSource, fragSource);

	// Getting the view/projection uniform
	vp_uni = box_sha->getUniform("vp");
}
kl::skybox::~skybox() {
	// Deleting the texture
	delete box_tex;

	// Deleting the mesh
	delete box_mes;

	// Deleting the shaders
	delete box_sha;
}

// Renders the cubemap
void kl::skybox::render(const kl::mat4& vpMat) const {
	// Setting the depth testing
	glDepthFunc(GL_LEQUAL);

	// Setting skybox uniforms
	vp_uni.setData(vpMat);

	// Drawing the cubemap
	box_tex->bind();
	box_mes->draw();

	// Resetting the depth testing
	glDepthFunc(GL_LESS);
}

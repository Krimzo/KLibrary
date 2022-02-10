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
const std::vector<kl::vertex3D> kl::skybox::boxVertices {
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

// Constructors/destructor
kl::skybox::skybox(const kl::image& fullbox) {
	// Checking the aspect ratio
	if (fullbox.getWidth() % 4 == 0 && fullbox.getHeight() % 3 == 0) {
		// Getting the part size
		const int partWidth = fullbox.getWidth() / 4;
		const int partHeight = fullbox.getHeight() / 3;

		// Checking the part size
		if (partWidth == partHeight) {
			// Extracting the sides
			const kl::ivec2 partSize(partWidth, partHeight);
			const kl::image front  = fullbox.getRect(partSize * kl::ivec2(1, 1), partSize * kl::ivec2(2, 2));
			const kl::image back   = fullbox.getRect(partSize * kl::ivec2(3, 1), partSize * kl::ivec2(4, 2));
			const kl::image left   = fullbox.getRect(partSize * kl::ivec2(0, 1), partSize * kl::ivec2(1, 2));
			const kl::image right  = fullbox.getRect(partSize * kl::ivec2(2, 1), partSize * kl::ivec2(3, 2));
			const kl::image top    = fullbox.getRect(partSize * kl::ivec2(1, 0), partSize * kl::ivec2(2, 1));
			const kl::image bottom = fullbox.getRect(partSize * kl::ivec2(1, 2), partSize * kl::ivec2(2, 3));

			// Calling the other constructor
			this->skybox::skybox(front, back, left, right, top, bottom);
		}
		else {
			printf("Skybox image width and height do not match!");
			std::cin.get();
			exit(69);
		}
	}
	else {
		printf("Skybox image does not have the correct aspect ratio!");
		std::cin.get();
		exit(69);
	}
}
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
	// Disabling depth testing
	kl::gl::setDepthTest(false);

	// Setting skybox uniforms
	vp_uni.setData(vpMat);

	// Binding the texture
	box_tex->bind();

	// Drawing the cubemap
	box_mes->draw();

	// Resetting depth testing
	kl::gl::setDepthTest(true);
}

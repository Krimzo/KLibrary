#include "KrimzLib/light/direct.h"


// Shadow vertex shader
const std::string vertSource = R"(
	#version 330

	layout (location = 0) in vec3 world;

	uniform mat4 sunVP;
	uniform mat4 w;

	void main() {
		gl_Position = sunVP * w * vec4(world, 1);
	}
)";

// Shadow fragment shader
const std::string fragSource = R"(
	#version 330

	void main() {} 
)";

// Returns the true light color
kl::vec4 kl::direct::getCol() const {
	return color * intensity;
}

// Returns the direction of light
kl::vec3 kl::direct::getDir() const {
	return direction.normalize();
}

// Generates the shadow buffers
void kl::direct::genBuff(int mapSize) {
	
}

// Destroys the shadow buffers
void kl::direct::delBuff() {
	
}

// Returns the light vp matrix
kl::mat4 kl::direct::matrix(const kl::camera& cam) const {
	// Calculating the near points
	const float nearDist = cam.nearPlane;
	const float Hnear = 2 * tan(kl::convert::toRadians(cam.fov) * 0.5f) * nearDist;
	const float Wnear = Hnear * cam.aspect;
	const kl::vec3 un = cam.getUp() * Hnear * 0.5;
	const kl::vec3 rn = cam.getRight() * Wnear * 0.5;
	const kl::vec3 centerNear = cam.position + cam.getForward() * nearDist;
	const kl::vec3 topLeftNear = centerNear + un - rn;
	const kl::vec3 topRightNear = centerNear + un + rn;
	const kl::vec3 bottomLeftNear = centerNear - un - rn;
	const kl::vec3 bottomRightNear = centerNear - un + rn;

	// Calculating the far points
	const float farDist = cam.shadowD;
	const float Hfar = 2 * tan(kl::convert::toRadians(cam.fov) * 0.5f) * farDist;
	const float Wfar = Hfar * cam.aspect;
	const kl::vec3 uf = cam.getUp() * Hfar * 0.5f;
	const kl::vec3 rf = cam.getRight() * Wfar * 0.5f;
	const kl::vec3 centerFar = cam.position + cam.getForward() * farDist;
	const kl::vec3 topLeftFar = centerFar + uf - rf;
	const kl::vec3 topRightFar = centerFar + uf + rf;
	const kl::vec3 bottomLeftFar = centerFar - uf - rf;
	const kl::vec3 bottomRightFar = centerFar - uf + rf;

	// Calculating the light view matrix
	const kl::mat4 view = kl::mat4::lookAt(getDir().negate(), vec3(0, 0, 0), vec3(0, 0, 1));

	// Transforming the frustum points to the light view space
	const std::vector<vec4> lightViewFrust{
		view * kl::vec4(bottomRightNear, 1),
		view * kl::vec4(topRightNear, 1),
		view * kl::vec4(bottomLeftNear, 1),
		view * kl::vec4(topLeftNear, 1),
		view * kl::vec4(bottomRightFar, 1),
		view * kl::vec4(topRightFar, 1),
		view * kl::vec4(bottomLeftFar, 1),
		view * kl::vec4(topLeftFar, 1)
	};

	// Finding the min and max points that generate the bounding box
	kl::vec3 minp( INFINITY,  INFINITY,  INFINITY);
	kl::vec3 maxp(-INFINITY, -INFINITY, -INFINITY);
	for (int i = 0; i < lightViewFrust.size(); i++) {
		if (lightViewFrust[i].x < minp.x) {
			minp.x = lightViewFrust[i].x;
		}
		if (lightViewFrust[i].y < minp.y) {
			minp.y = lightViewFrust[i].y;
		}
		if (lightViewFrust[i].z < minp.z) {
			minp.z = lightViewFrust[i].z;
		}

		if (lightViewFrust[i].x > maxp.x) {
			maxp.x = lightViewFrust[i].x;
		}
		if (lightViewFrust[i].y > maxp.y) {
			maxp.y = lightViewFrust[i].y;
		}
		if (lightViewFrust[i].z > maxp.z) {
			maxp.z = lightViewFrust[i].z;
		}
	}

	// Calculating the ortho projection matrix
	const kl::mat4 proj = kl::mat4::ortho(minp.x, maxp.x, minp.y, maxp.y, -maxp.z - 3 * cam.shadowD, -minp.z);

	// Setting the sun view/projection matrix
	return proj * view;
}

void kl::direct::renderShadows() const {

}

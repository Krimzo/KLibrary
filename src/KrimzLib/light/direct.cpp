#include "KrimzLib/light/direct.h"


// Returns the true light color
kl::vec4 kl::direct::getCol() const {
	return color * intensity;
}

// Returns the direction of light
kl::vec3 kl::direct::getDir() const {
	return direction.normalize();
}

// Returns the light vp matrix
kl::mat4 kl::direct::matrix(const kl::camera& cam) const {
	// Inverse camera matrix calculation
	const kl::mat4 invCam = (
		kl::mat4::persp(cam.fov, cam.aspect, cam.nearPlane, cam.shadowDis) *
		kl::mat4::lookAt(cam.position, cam.position + cam.getForward(), cam.getUp())
	).inverse();

	// Frustum world corners
	std::vector<kl::vec4> frustumCorners;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				const kl::vec4 corner = invCam * kl::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(corner / corner.w);
			}
		}
	}

	// Centroid calculation
	kl::vec3 centroid;
	for (const auto& corn : frustumCorners) {
		centroid += corn.xyz();
	}
	centroid /= 8.0f;

	// Light view matrix
	const kl::mat4 view = kl::mat4::lookAt(centroid - this->getDir(), centroid, kl::vec3::pos_y);

	// Finding min and max points
	kl::vec3 minp(FLT_MAX, FLT_MAX, FLT_MAX);
	kl::vec3 maxp(FLT_MIN, FLT_MIN, FLT_MIN);
	for (const auto& corn : frustumCorners) {
		const kl::vec4 lightCorn = view * corn;
		minp.x = min(minp.x, lightCorn.x);
		maxp.x = max(maxp.x, lightCorn.x);
		minp.y = min(minp.y, lightCorn.y);
		maxp.y = max(maxp.y, lightCorn.y);
		minp.z = min(minp.z, lightCorn.z);
		maxp.z = max(maxp.z, lightCorn.z);
	}

	// Tunning near plane
	const float shadowMulti = 5.0f;
	maxp.z *= shadowMulti;

	// Light proj matrix
	const kl::mat4 proj = kl::mat4::ortho(minp.x, maxp.x, minp.y, maxp.y, maxp.z, minp.z);

	// Return
	return proj * view;
}

#include "render/camera.cuh"


// Camera direction getters
ALL kl::float3 kl::camera::forward() const {
	return direction.norm();
}
ALL kl::float3 kl::camera::right() const {
	return kl::float3(0.0f, 1.0f, 0.0f).cross(forward()).norm();
}
ALL kl::float3 kl::camera::up() const {
	return forward().cross(right()).norm();
}

// Camera movement
ALL void kl::camera::moveForward(float deltaTime) {
	position = position + forward() * (speed * deltaTime);
}
ALL void kl::camera::moveBack(float deltaTime) {
	position = position - forward() * (speed * deltaTime);
}
ALL void kl::camera::moveRight(float deltaTime) {
	position = position + right() * (speed * deltaTime);
}
ALL void kl::camera::moveLeft(float deltaTime) {
	position = position - right() * (speed * deltaTime);
}
ALL void kl::camera::moveUp(float deltaTime) {
	position = position + kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
}
ALL void kl::camera::moveDown(float deltaTime) {
	position = position - kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
}

// Camera rotation
ALL void kl::camera::rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit) {
	const kl::float2 rotation = (mousePos - frameCenter) * sens;
	const kl::float3 forwardVert = forward().rotate(rotation.y, right());
	if (std::abs(forwardVert.angle(kl::float3(0.0f, 1.0f, 0.0f)) - 90.0f) <= verticalAngleLimit) {
		direction = forwardVert;
	}
	direction = forward().rotate(rotation.x, kl::float3(0.0f, 1.0f, 0.0f));
}

// Camera matrix
ALL kl::mat4 kl::camera::matrix() const {
	const kl::mat4 view = kl::mat4::lookAt(position, position + forward(), kl::float3(0.0f, 1.0f, 0.0f));
	const kl::mat4 proj = kl::mat4::persp(fov, aspect, near, far);
	return proj * view;
}

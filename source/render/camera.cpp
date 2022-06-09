#include "render/camera.h"


// Camera direction getters
kl::float3 kl::camera::getForward() const {
	return forward.norm();
}
kl::float3 kl::camera::getRight() const {
	return kl::float3(0.0f, 1.0f, 0.0f).cross(getForward()).norm();
}
kl::float3 kl::camera::getUp() const {
	return getForward().cross(getRight()).norm();
}

// Camera movement
void kl::camera::moveForward(float deltaTime) {
	position = position + getForward() * (speed * deltaTime);
}
void kl::camera::moveBack(float deltaTime) {
	position = position - getForward() * (speed * deltaTime);
}
void kl::camera::moveRight(float deltaTime) {
	position = position + getRight() * (speed * deltaTime);
}
void kl::camera::moveLeft(float deltaTime) {
	position = position - getRight() * (speed * deltaTime);
}
void kl::camera::moveUp(float deltaTime) {
	position = position + kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
}
void kl::camera::moveDown(float deltaTime) {
	position = position - kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
}

// Camera rotation
void kl::camera::rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit) {
	const kl::float2 rotation = (mousePos - frameCenter) * sens;
	const kl::float3 forwardVert = getForward().rotate(rotation.y, getRight());
	if (std::abs(forwardVert.angle({ 0.0f, 1.0f, 0.0f }) - 90.0f) <= verticalAngleLimit) {
		forward = forwardVert;
	}
	forward = getForward().rotate(rotation.x, { 0.0f, 1.0f, 0.0f });
}

// Camera matrix
kl::mat4 kl::camera::matrix() const {
	const kl::mat4 view = kl::mat4::lookAt(position, position + getForward(), { 0.0f, 1.0f, 0.0f });
	const kl::mat4 proj = kl::mat4::persp(fov, aspect, near, far);
	return proj * view;
}

#include "render/camera.h"


static const kl::float3 globalUP = { 0.0f, 1.0f, 0.0f };

// Camera direction getters
kl::float3 kl::camera::forward() const {
	return direction.norm();
}
kl::float3 kl::camera::right() const {
	return globalUP.cross(forward()).norm();
}
kl::float3 kl::camera::up() const {
	return forward().cross(right()).norm();
}

// Camera movement
void kl::camera::moveForward(float deltaTime) {
	position = position + forward() * (speed * deltaTime);
}
void kl::camera::moveBack(float deltaTime) {
	position = position - forward() * (speed * deltaTime);
}
void kl::camera::moveRight(float deltaTime) {
	position = position + right() * (speed * deltaTime);
}
void kl::camera::moveLeft(float deltaTime) {
	position = position - right() * (speed * deltaTime);
}
void kl::camera::moveUp(float deltaTime) {
	position = position + globalUP * (speed * deltaTime);
}
void kl::camera::moveDown(float deltaTime) {
	position = position - globalUP * (speed * deltaTime);
}

// Camera rotation
void kl::camera::rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit) {
	const kl::float2 rotation = (mousePos - frameCenter) * sens;
	const kl::float3 forwardVert = forward().rotate(rotation.y, right());
	if (std::abs(forwardVert.angle(globalUP) - 90.0f) <= verticalAngleLimit) {
		direction = forwardVert;
	}
	direction = forward().rotate(rotation.x, globalUP);
}

// Camera matrix
kl::mat4 kl::camera::matrix() const {
	const kl::mat4 view = kl::mat4::lookAt(position, position + forward(), globalUP);
	const kl::mat4 proj = kl::mat4::persp(fov, aspect, near, far);
	return proj * view;
}

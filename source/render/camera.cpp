#include "render/camera.h"


static const kl::float3 globalUP = { 0.0f, 1.0f, 0.0f };

void kl::camera::forward(const kl::float3& dir) {
	m_Forward = dir.normalize();
	m_Right = globalUP.cross(m_Forward).normalize();
	m_Up = m_Forward.cross(m_Right).normalize();
}
kl::float3 kl::camera::forward() const {
	return m_Forward;
}
kl::float3 kl::camera::right() const {
	return m_Right;
}
kl::float3 kl::camera::up() const {
	return m_Up;
}

// Camera movement
void kl::camera::moveForward(float deltaTime) {
	position = position + m_Forward * (speed * deltaTime);
}
void kl::camera::moveBack(float deltaTime) {
	position = position - m_Forward * (speed * deltaTime);
}
void kl::camera::moveRight(float deltaTime) {
	position = position + m_Right * (speed * deltaTime);
}
void kl::camera::moveLeft(float deltaTime) {
	position = position - m_Right * (speed * deltaTime);
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
	const kl::float3 forwardVert = m_Forward.rotate(rotation.y, m_Right);
	if (std::abs(forwardVert.angle(globalUP) - 90.0f) <= verticalAngleLimit) {
		forward(forwardVert);
	}
	forward(m_Forward.rotate(rotation.x, globalUP));
}

// Camera matrix
kl::mat4 kl::camera::matrix() const {
	const kl::mat4 view = kl::mat4::lookAt(position, position + m_Forward, globalUP);
	const kl::mat4 proj = kl::mat4::persp(fov, aspect, near, far);
	return proj * view;
}

#include "Render/Camera.h"


static const kl::Float3 GlobalUP = { 0.0f, 1.0f, 0.0f };

void kl::Camera::setForward(const Float3& dir) {
	m_Forward = dir.normalize();
	m_Right = GlobalUP.cross(m_Forward).normalize();
	m_Up = m_Forward.cross(m_Right).normalize();
}

kl::Float3 kl::Camera::getForward() const {
	return m_Forward;
}

kl::Float3 kl::Camera::getRight() const {
	return m_Right;
}

kl::Float3 kl::Camera::getUp() const {
	return m_Up;
}

void kl::Camera::moveForward(float deltaTime) {
	position = position + m_Forward * (speed * deltaTime);
}

void kl::Camera::moveBack(float deltaTime) {
	position = position - m_Forward * (speed * deltaTime);
}

void kl::Camera::moveRight(float deltaTime) {
	position = position + m_Right * (speed * deltaTime);
}

void kl::Camera::moveLeft(float deltaTime) {
	position = position - m_Right * (speed * deltaTime);
}

void kl::Camera::moveUp(float deltaTime) {
	position = position + GlobalUP * (speed * deltaTime);
}

void kl::Camera::moveDown(float deltaTime) {
	position = position - GlobalUP * (speed * deltaTime);
}

void kl::Camera::rotate(const Float2& mousePos, const Float2& frameCenter, float verticalAngleLimit) {
	const Float2 rotation = (mousePos - frameCenter) * sensitivity;
	const Float3 forwardVert = m_Forward.rotate(rotation.y, m_Right);

	if (std::abs(forwardVert.angle(GlobalUP) - 90.0f) <= verticalAngleLimit) {
		setForward(forwardVert);
	}

	setForward(m_Forward.rotate(rotation.x, GlobalUP));
}

kl::Mat4 kl::Camera::matrix() const {
	const Mat4 view = Mat4::LookAt(position, position + m_Forward, GlobalUP);
	const Mat4 proj = Mat4::Perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	return proj * view;
}

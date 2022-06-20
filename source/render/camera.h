#pragma once 

#include "math/math.h"

#undef near
#undef far

namespace kl {
	class camera {
	private:
		kl::float3 m_Forward = { 0.0f, 0.0f, 1.0f };
		kl::float3 m_Right = { 1.0f, 0.0f, 0.0f };
		kl::float3 m_Up = { 0.0f, 1.0f, 0.0f };

	public:
		kl::float3 position;
		float aspect = 1.7778f;
		float fov = 75.0f;
		float near = 0.01f;
		float far = 500.0f;
		float speed = 2.0f;
		float sens = 0.1f;

		ALL void forward(const kl::float3& dir) {
			m_Forward = dir.normalize();
			m_Right = kl::float3(0.0f, 1.0f, 0.0f).cross(m_Forward).normalize();
			m_Up = m_Forward.cross(m_Right).normalize();
		}
		ALL kl::float3 forward() const {
			return m_Forward;
		}
		ALL kl::float3 right() const {
			return m_Right;
		}
		ALL kl::float3 up() const {
			return m_Up;
		}

		ALL void moveForward(float deltaTime) {
			position = position + m_Forward * (speed * deltaTime);
		}
		ALL void moveBack(float deltaTime) {
			position = position - m_Forward * (speed * deltaTime);
		}
		ALL void moveRight(float deltaTime) {
			position = position + m_Right * (speed * deltaTime);
		}
		ALL void moveLeft(float deltaTime) {
			position = position - m_Right * (speed * deltaTime);
		}
		ALL void moveUp(float deltaTime) {
			position = position + kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
		}
		ALL void moveDown(float deltaTime) {
			position = position - kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
		}

		ALL void rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit = 85.0f) {
			const kl::float2 rotation = (mousePos - frameCenter) * sens;
			const kl::float3 forwardVert = m_Forward.rotate(rotation.y, m_Right);
			if (std::abs(forwardVert.angle(kl::float3(0.0f, 1.0f, 0.0f)) - 90.0f) <= verticalAngleLimit) {
				forward(forwardVert);
			}
			forward(m_Forward.rotate(rotation.x, kl::float3(0.0f, 1.0f, 0.0f)));
		}

		ALL kl::mat4 matrix() const {
			const kl::mat4 view = kl::mat4::lookAt(position, position + m_Forward, kl::float3(0.0f, 1.0f, 0.0f));
			const kl::mat4 proj = kl::mat4::persp(fov, aspect, near, far);
			return proj * view;
		}
	};
}

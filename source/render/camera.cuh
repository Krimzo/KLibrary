#pragma once 

#include "math/math.cuh"

#undef near
#undef far

namespace kl {
	class camera {
	public:
		kl::float3 direction = { 0.0f, 0.0f, 1.0f };
		kl::float3 position;
		float aspect = 1.7778f;
		float fov = 75.0f;
		float near = 0.01f;
		float far = 500.0f;
		float speed = 2.0f;
		float sens = 0.1f;

		ALL kl::float3 forward() const {
			return direction.norm();
		}
		ALL kl::float3 right() const {
			return kl::float3(0.0f, 1.0f, 0.0f).cross(forward()).norm();
		}
		ALL kl::float3 up() const {
			return forward().cross(right()).norm();
		}

		ALL void moveForward(float deltaTime) {
			position = position + forward() * (speed * deltaTime);
		}
		ALL void moveBack(float deltaTime) {
			position = position - forward() * (speed * deltaTime);
		}
		ALL void moveRight(float deltaTime) {
			position = position + right() * (speed * deltaTime);
		}
		ALL void moveLeft(float deltaTime) {
			position = position - right() * (speed * deltaTime);
		}
		ALL void moveUp(float deltaTime) {
			position = position + kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
		}
		ALL void moveDown(float deltaTime) {
			position = position - kl::float3(0.0f, 1.0f, 0.0f) * (speed * deltaTime);
		}

		ALL void rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit = 85.0f) {
			const kl::float2 rotation = (mousePos - frameCenter) * sens;
			const kl::float3 forwardVert = forward().rotate(rotation.y, right());
			if (std::abs(forwardVert.angle(kl::float3(0.0f, 1.0f, 0.0f)) - 90.0f) <= verticalAngleLimit) {
				direction = forwardVert;
			}
			direction = forward().rotate(rotation.x, kl::float3(0.0f, 1.0f, 0.0f));
		}

		ALL kl::mat4 matrix() const {
			const kl::mat4 view = kl::mat4::lookAt(position, position + forward(), kl::float3(0.0f, 1.0f, 0.0f));
			const kl::mat4 proj = kl::mat4::persp(fov, aspect, near, far);
			return proj * view;
		}
	};
}

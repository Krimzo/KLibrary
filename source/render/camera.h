#pragma once 

#include "math/math.h"

#undef near
#undef far

namespace kl {
	class camera {
	public:
		kl::float3 forward = { 0.0f, 0.0f, 1.0f };
		kl::float3 position;
		float aspect = 1.7778f;
		float fov = 75.0f;
		float near = 0.01f;
		float far = 500.0f;
		float speed = 2.0f;
		float sens = 0.1f;

		// Camera direction getters
		kl::float3 getForward() const;
		kl::float3 getRight() const;
		kl::float3 getUp() const;

		// Camera movement
		void moveForward(float deltaTime);
		void moveBack(float deltaTime);
		void moveRight(float deltaTime);
		void moveLeft(float deltaTime);
		void moveUp(float deltaTime);
		void moveDown(float deltaTime);

		// Camera rotation
		void rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit = 85.0f);

		// Computes and returns the camera matrix
		kl::mat4 matrix() const;
	};
}

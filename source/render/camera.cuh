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

		ALL kl::float3 forward() const;
		ALL kl::float3 right() const;
		ALL kl::float3 up() const;

		ALL void moveForward(float deltaTime);
		ALL void moveBack(float deltaTime);
		ALL void moveRight(float deltaTime);
		ALL void moveLeft(float deltaTime);
		ALL void moveUp(float deltaTime);
		ALL void moveDown(float deltaTime);

		ALL void rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit = 85.0f);

		ALL kl::mat4 matrix() const;
	};
}

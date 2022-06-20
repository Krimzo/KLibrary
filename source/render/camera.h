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

		void forward(const kl::float3& dir);
		kl::float3 forward() const;
		kl::float3 right() const;
		kl::float3 up() const;

		void moveForward(float deltaTime);
		void moveBack(float deltaTime);
		void moveRight(float deltaTime);
		void moveLeft(float deltaTime);
		void moveUp(float deltaTime);
		void moveDown(float deltaTime);

		void rotate(const kl::float2& mousePos, const kl::float2& frameCenter, float verticalAngleLimit = 85.0f);

		kl::mat4 matrix() const;
	};
}

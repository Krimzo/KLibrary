#pragma once 

#include "math/float3.h"
#include "math/mat4.h"

#undef near
#undef far

namespace kl
{
	class camera
	{
	public:
		kl::float3 forward = kl::float3::pos_z;
		kl::float3 position;
		float aspect = 1.777f;
		float near = 0.01f;
		float fov = 75.0f;
		float far = 100.0f;
		float shadows = 40.0f;
		float speed = 2.0f;
		float sens = 0.025f;

		// Constructor
		camera();

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
		void rotate(const kl::int2& mousePos, const kl::int2& frameCenter, float verticalAngleLimit = 85);

		// Computes and returns the camera matrix
		kl::mat4 matrix() const;
	};
}

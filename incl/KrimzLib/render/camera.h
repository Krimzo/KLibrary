#pragma once 

#include "KrimzLib/math/float3.h"
#include "KrimzLib/math/mat4.h"


namespace kl {
	class camera {
	public:
		kl::float3 forward = kl::float3::pos_z;
		kl::float3 position;
		float nearPlane = 0.01f;
		float farPlane = 100.0f;
		float aspect = 16.0f / 9;
		float fov = 75.0f;
		float speed = 2.0f;
		float sens = 0.025f;
		float shadowDis = 40.0f;

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

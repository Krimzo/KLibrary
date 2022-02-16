#pragma once 

#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/mat4.h"


namespace kl {
	class camera {
	public:
		kl::vec3 forward = kl::vec3(0, 0, 1);
		kl::vec3 position = kl::vec3(0, 0, 0);
		float nearPlane = 0.01f;
		float farPlane = 100.0f;
		float aspect = 16.0f / 9;
		float fov = 75.0f;
		float speed = 2.0f;
		float sens = 0.025f;
		float shadowD = 20.0f;

		// Constructor
		camera();

		// Camera direction getters
		kl::vec3 getForward() const;
		kl::vec3 getRight() const;
		kl::vec3 getUp() const;

		// Camera movement
		void moveForward(float deltaTime);
		void moveBack(float deltaTime);
		void moveRight(float deltaTime);
		void moveLeft(float deltaTime);
		void moveUp(float deltaTime);
		void moveDown(float deltaTime);

		// Camera rotation
		void rotate(const kl::ivec2& mousePos, const kl::ivec2& frameCenter, float verticalAngleLimit = 85);

		// Computes and returns the camera matrix
		kl::mat4 matrix() const;
	};
}

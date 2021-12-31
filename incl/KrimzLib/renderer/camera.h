#pragma once 


namespace kl {
	struct camera {
		kl::vec3 position;
		float nearPlane = 1;
		float farPlane = 10;
		float width = 600;
		float height = 600;
		float fov = 75;
		float speed = 2;
		float sens = 0.05;
		float shadowD = 25;

		// Camera direction getters
		kl::vec3 getForward() {
			return forward.normalize();
		}
		kl::vec3 getRight() {
			return yAxis.cross(getForward()).normalize();
		}
		kl::vec3 getUp() {
			return getForward().cross(getRight()).normalize();
		}

		// Camera movement
		void moveForward(float deltaTime) {
			position = position + getForward() * (speed * deltaTime);
		}
		void moveBack(float deltaTime) {
			position = position - getForward() * (speed * deltaTime);
		}
		void moveRight(float deltaTime) {
			position = position + getRight() * (speed * deltaTime);
		}
		void moveLeft(float deltaTime) {
			position = position - getRight() * (speed * deltaTime);
		}
		void moveUp(float deltaTime) {
			position = position + yAxis * (speed * deltaTime);
		}
		void moveDown(float deltaTime) {
			position = position - yAxis * (speed * deltaTime);
		}

		// Camera rotation
		void rotate(kl::ivec2 mousePos, kl::ivec2 frameCenter, float verticalAngleLimit = 85) {
			// Calculating the mouse movement
			const int dx = mousePos.x - frameCenter.x;
			const int dy = mousePos.y - frameCenter.y;

			// Calculating the x and y rotation
			const float xRotation = dx * sens;
			const float yRotation = dy * sens;

			// Calculating the vertically rotated forward vector
			kl::vec3 forwardVert = getForward().rotate(yRotation, getRight());

			// Checking if the vertical rotation is goin to be inside the bounds
			if (std::abs(forwardVert.angle(yAxis) - 90) <= verticalAngleLimit) {
				forward = forwardVert;
			}

			// Calculating the horizontally rotated forward vector
			forward = getForward().rotate(xRotation, kl::vec3(0, 1, 0));
		}

		// Computes and returns the camera matrix
		kl::mat4 matrix() {
			// Building the proj matrix
			kl::mat4 proj = kl::mat4::perspective(fov, width, height, nearPlane, farPlane);

			// Building the view matrix
			kl::mat4 view = kl::mat4::view(getForward(), getRight(), getUp(), position);

			// Multiplying and returning
			return proj * view;
		}

	private:
		// Variables
		static kl::vec3 yAxis;
		kl::vec3 forward = kl::vec3(0, 0, 1);
	};
}
kl::vec3 kl::camera::yAxis = kl::vec3(0, 1, 0);
#pragma once 


namespace kl {
    struct camera {
		kl::vec3 position;
		float sensitivity;
		float speed;

		// Constructors
		camera() {
			position = kl::vec3(0, 0, 0);
			forward = kl::vec3(0, 0, 1);
			sensitivity = 0.05f;
			speed = 2;
			setFOV(75);
			setAspect(1);
			setPlanes(1, 10);
		}

		// Camera direction getters
		kl::vec3 getForward() {
			return forward.normalize();
		}
		kl::vec3 getRight() {
			return kl::vec3(0, 1, 0).cross(forward).normalize();
		}
		kl::vec3 getUp() {
			return forward.cross(getRight()).normalize();
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
			position = position + kl::vec3(0, 1, 0) * (speed * deltaTime);
		}
		void moveDown(float deltaTime) {
			position = position - kl::vec3(0, 1, 0) * (speed * deltaTime);
		}

		// Camera rotation
		void rotate(kl::point frameCenter, kl::point mousePos, float verticalAngleLimit = 85) {
			// Calculating the mouse movement
			const int dx = mousePos.x - frameCenter.x;
			const int dy = mousePos.y - frameCenter.y;

			// Calculating the x and y rotation
			const float xRotation = dx * sensitivity;
			const float yRotation = dy * sensitivity;

			// Calculating the vertically rotated forward vector
			kl::vec3 forwardVert = forward.rotate(yRotation, getRight());

			// Checking if the vertical rotation is goin to be inside the bounds
			if (std::abs(kl::convert::toDegrees(forwardVert.angle(kl::vec3(0, 1, 0))) - 90) <= verticalAngleLimit) {
				forward = forwardVert;
			}

			// Calculating the horizontally rotated forward vector
			forward = forward.rotate(xRotation, kl::vec3(0, 1, 0));
		}

		// Computes and returns the camera transformation matrix
		kl::mat4 viewMatrix() {
			// Getting the direction vectors
			const kl::vec3 u = getRight();
			const kl::vec3 v = getUp();
			const kl::vec3 n = getForward();

			// Building the view matrix
			kl::mat4 rotation;
			rotation[ 0] = u.x;
			rotation[ 1] = u.y;
			rotation[ 2] = u.z;
			rotation[ 4] = v.x;
			rotation[ 5] = v.y;
			rotation[ 6] = v.z;
			rotation[ 8] = n.x;
			rotation[ 9] = n.y;
			rotation[10] = n.z;
			return rotation * kl::mat4::translate(position.negate());
		}

		// Computes and stores the tan const
		void setFOV(float fov) {
			tanHalf = 1 / tan(kl::convert::toRadians(fov * 0.5f));
		}

		// Computes and stores the aspect ratio
		void setAspect(float a) {
			aspectRec = 1 / a;
		}
		void setAspect(kl::size frameSize) {
			aspectRec = (float)frameSize.height / frameSize.width;
		}

		// Computes and stores the near and far plane calculations
		void setPlanes(float nearPlane, float farPlane) {
			planesA = (-farPlane - nearPlane) / (nearPlane - farPlane);
			planesB = (2 * nearPlane * farPlane) / (nearPlane - farPlane);
		}

		// Computes and returns the camera projection matrix
		kl::mat4 projMatrix() {
			kl::mat4 temp;
			temp[0] = tanHalf * aspectRec;
			temp[5] = tanHalf;
			temp[10] = planesA;
			temp[11] = planesB;
			temp[14] = 1;
			temp[15] = 0;
			return temp;
		}

	private:
		// Variables
		kl::vec3 forward;
		float tanHalf;
		float aspectRec;
		float planesA;
		float planesB;
	};
}
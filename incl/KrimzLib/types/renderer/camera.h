#pragma once 


namespace kl {
    struct camera {
		kl::vec3 position;
		float sensitivity = 0.05f;
		float speed = 2;

		// Constructors
		camera() {
			setFOV(75);
			setAspect(1);
			setPlanes(1, 10);
		}

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
		void rotate(kl::point mousePos, kl::point frameCenter, float verticalAngleLimit = 85) {
			// Calculating the mouse movement
			const int dx = mousePos.x - frameCenter.x;
			const int dy = mousePos.y - frameCenter.y;

			// Calculating the x and y rotation
			const float xRotation = dx * sensitivity;
			const float yRotation = dy * sensitivity;

			// Calculating the vertically rotated forward vector
			kl::vec3 forwardVert = getForward().rotate(yRotation, getRight());

			// Checking if the vertical rotation is goin to be inside the bounds
			if (std::abs(kl::convert::toDegrees(forwardVert.angle(yAxis)) - 90) <= verticalAngleLimit) {
				forward = forwardVert;
			}

			// Calculating the horizontally rotated forward vector
			forward = getForward().rotate(xRotation, kl::vec3(0, 1, 0));
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

		// Computes and returns the camera matrix
		kl::mat4 matrix() {
			// Building the proj matrix
			kl::mat4 proj;
			proj[ 0] = tanHalf * aspectRec;
			proj[ 5] = tanHalf;
			proj[10] = planesA;
			proj[11] = planesB;
			proj[14] = 1;
			proj[15] = 0;

			// Getting the direction vectors
			const kl::vec3 u = getRight();
			const kl::vec3 v = getUp();
			const kl::vec3 n = getForward();

			// Building the view matrix
			kl::mat4 view;
			view[ 0] = u.x;
			view[ 1] = u.y;
			view[ 2] = u.z;
			view[ 4] = v.x;
			view[ 5] = v.y;
			view[ 6] = v.z;
			view[ 8] = n.x;
			view[ 9] = n.y;
			view[10] = n.z;
			view = view * kl::mat4::translate(position.negate());

			// Multiplying and returning
			return proj * view;
		}

	private:
		// Variables
		static kl::vec3 yAxis;
		kl::vec3 forward = kl::vec3(0, 0, 1);
		float tanHalf = 0;
		float aspectRec = 0;
		float planesA = 0;
		float planesB = 0;
	};
	kl::vec3 camera::yAxis = kl::vec3(0, 1, 0);
}
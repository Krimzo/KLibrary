#pragma once 


namespace kl {
    struct camera {
		kl::vec3 position;
		float speed;
		float sensitivity;

		// Constructors
		camera() {
			position = kl::vec3(0, 0, 0);
			forward = kl::vec3(0, 0, 1);
			right = kl::vec3(1, 0, 0);
			up = kl::vec3(0, 1, 0);
			speed = 2;
			sensitivity = 1;
			setFOV(90);
			setAspect(kl::size(1, 1));
			setPlanes(1, 10);
		}

		// Direction getters
		kl::vec3 getForward() {
			return forward;
		}
		kl::vec3 getRight() {
			return right;
		}
		kl::vec3 getUp() {
			return up;
		}

		// Direction setters
		void setForward(kl::vec3 val) {
			forward = val.normalize();
			right = up.cross(forward);
		}
		void setUp(kl::vec3 val) {
			up = val.normalize();
			right = up.cross(forward);
		}

		// Camera movement
		void moveForward(float deltaTime) {
			position = position + forward * (speed * deltaTime);
		}
		void moveBack(float deltaTime) {
			position = position - forward * (speed * deltaTime);
		}
		void moveRight(float deltaTime) {
			position = position + right * (speed * deltaTime);
		}
		void moveLeft(float deltaTime) {
			position = position - right * (speed * deltaTime);
		}
		void moveUp(float deltaTime) {
			position = position + up * (speed * deltaTime);
		}
		void moveDown(float deltaTime) {
			position = position - up * (speed * deltaTime);
		}

		// Computes and returns the camera transformation matrix
		kl::mat4 viewMatrix() {
			kl::mat4 rotation;
			rotation[ 0] = right.x;
			rotation[ 1] = right.y;
			rotation[ 2] = right.z;
			rotation[ 4] = up.x;
			rotation[ 5] = up.y;
			rotation[ 6] = up.z;
			rotation[ 8] = forward.x;
			rotation[ 9] = forward.y;
			rotation[10] = forward.z;
			return rotation * kl::mat4::translate(position * (-1));
		}

		// Computes and stores the tan const
		void setFOV(float fov) {
			tanHalf = 1 / tan(kl::convert::toRadians(fov * 0.5f));
		}

		// Computes and stores the aspect ratio
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
		kl::vec3 right;
		kl::vec3 up;
		float tanHalf;
		float aspectRec;
		float planesA;
		float planesB;
	};
}
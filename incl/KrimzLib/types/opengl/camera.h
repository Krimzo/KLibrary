#pragma once 


namespace kl {
    struct camera {
		kl::vec3 position;
		kl::vec3 u;
		kl::vec3 v;
		kl::vec3 n;

		// Constructors
		camera() {
			position = kl::vec3();
			u = kl::vec3(1, 0, 0);
			v = kl::vec3(0, 1, 0);
			n = kl::vec3(0, 0, 1);
			updateFOV(90);
			updateAspect(kl::size(1, 1));
			updatePlanes(1, 10);
		}

		// Computes and returns the camera transformation matrix
		kl::mat4 getViewMatrix() {
			kl::mat4 temp;
			temp[ 0] = u.x;
			temp[ 1] = u.y;
			temp[ 2] = u.z;
			temp[ 3] = -position.x;
			temp[ 4] = v.x;
			temp[ 5] = v.y;
			temp[ 6] = v.z;
			temp[ 7] = -position.y;
			temp[ 8] = n.x;
			temp[ 9] = n.y;
			temp[10] = n.z;
			temp[11] = -position.z;
			return temp;
		}

		// Computes and stores the tan const
		void updateFOV(float fov) {
			tanHalf = 1 / tan(kl::convert::toRadians(fov * 0.5f));
		}

		// Computes and stores the aspect ratio
		void updateAspect(kl::size frameSize) {
			aspectRec = (float)frameSize.height / frameSize.width;
		}

		// Computes and stores the near and far plane calculations
		void updatePlanes(float nearPlane, float farPlane) {
			planesA = (-farPlane - nearPlane) / (nearPlane - farPlane);
			planesB = (2 * nearPlane * farPlane) / (nearPlane - farPlane);
		}

		// Computes and returns the camera projection matrix
		kl::mat4 getProjMatrix() {
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
		float tanHalf;
		float aspectRec;
		float planesA;
		float planesB;
	};
}
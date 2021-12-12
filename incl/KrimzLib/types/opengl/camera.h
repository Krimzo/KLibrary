#pragma once 


namespace kl {
    struct camera {
		kl::vec3 position;
		kl::vec3 rotation;

		// Constructors
		camera() {
			position = kl::vec3();
			rotation = kl::vec3();
			updateFOV(90);
			updateAspect(kl::size(1, 1));
			updatePlanes(1, 10);
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

		// Computes and returns the camera transformation/projection matrix
		kl::mat4 getMatrix() {
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
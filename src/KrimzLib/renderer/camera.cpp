#include "KrimzLib/renderer/camera.h"


// Constructor
kl::camera::camera() {

}

// Camera direction getters
kl::vec3 kl::camera::getForward() const {
	return forward.normalize();
}
kl::vec3 kl::camera::getRight() const {
	return kl::vec3::pos_y.cross(getForward()).normalize();
}
kl::vec3 kl::camera::getUp() const {
	return getForward().cross(getRight()).normalize();
}

// Camera movement
void kl::camera::moveForward(float deltaTime) {
	position = position + getForward() * (speed * deltaTime);
}
void kl::camera::moveBack(float deltaTime) {
	position = position - getForward() * (speed * deltaTime);
}
void kl::camera::moveRight(float deltaTime) {
	position = position + getRight() * (speed * deltaTime);
}
void kl::camera::moveLeft(float deltaTime) {
	position = position - getRight() * (speed * deltaTime);
}
void kl::camera::moveUp(float deltaTime) {
	position = position + kl::vec3::pos_y * (speed * deltaTime);
}
void kl::camera::moveDown(float deltaTime) {
	position = position - kl::vec3::pos_y * (speed * deltaTime);
}

// Camera rotation
void kl::camera::rotate(const kl::ivec2& mousePos, const kl::ivec2& frameCenter, float verticalAngleLimit) {
	// Calculating the mouse movement
	const int dx = mousePos.x - frameCenter.x;
	const int dy = mousePos.y - frameCenter.y;

	// Calculating the x and y rotation
	const float xRotation = dx * sens;
	const float yRotation = dy * sens;

	// Calculating the vertically rotated forward vector
	kl::vec3 forwardVert = getForward().rotate(yRotation, getRight());

	// Checking if the vertical rotation is goin to be inside the bounds
	if (std::abs(forwardVert.angle(kl::vec3::pos_y) - 90.0f) <= verticalAngleLimit) {
		forward = forwardVert;
	}

	// Calculating the horizontally rotated forward vector
	forward = getForward().rotate(xRotation, kl::vec3::pos_y);
}

// Computes and returns the camera matrix
kl::mat4 kl::camera::matrix() const {
	// Building the view matrix
	const kl::mat4 view = kl::mat4::lookAt(position, position + getForward(), getUp());

	// Building the proj matrix
	const kl::mat4 proj = kl::mat4::persp(fov, aspect, nearPlane, farPlane);

	// Multiplying and returning
	return proj * view;
}

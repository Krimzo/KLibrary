#pragma once 

#include "Math/Math.h"

#undef near
#undef far

namespace kl {
	class Camera {
		Float3 m_Forward = { 0.0f, 0.0f, 1.0f };
		Float3 m_Right = { 1.0f, 0.0f, 0.0f };
		Float3 m_Up = { 0.0f, 1.0f, 0.0f };

	public:
		Float3 position;
		float aspectRatio = 1.7778f;
		float fieldOfView = 75.0f;
		float nearPlane = 0.01f;
		float farPlane = 500.0f;
		float sensitivity = 0.1f;
		float speed = 2.0f;

		template<typename T>
		void updateAspectRatio(const Vector2<T>& size) {
			aspectRatio = float(size.x) / float(size.y);
		}

		void setForward(const Float3& dir);

		Float3 getForward() const;
		Float3 getRight() const;
		Float3 getUp() const;

		void moveForward(float deltaTime);
		void moveBack(float deltaTime);
		void moveRight(float deltaTime);
		void moveLeft(float deltaTime);
		void moveUp(float deltaTime);
		void moveDown(float deltaTime);

		void rotate(const Float2& mousePos, const Float2& frameCenter, float verticalAngleLimit = 85.0f);

		Mat4 matrix() const;
	};
}
